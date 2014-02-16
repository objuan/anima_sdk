#include "Max.h"
#include <stdio.h>
#include <direct.h>
#include <commdlg.h>
#include "utilapi.h"
#include "AnimaExp.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include <IPathConfigMgr.h> 

#include <maxheapdirect.h>

HINSTANCE hInstance;
HMODULE hModule = NULL;

static BOOL showPrompts;

char anima_programPath[1000];

// 3DS-to-MAX time constant multiplier
#define TIME_CONSTANT GetTicksPerFrame()

// Some handy macros
static float msc_wk;

void
split_fn(char *path,char *file,char *pf)
	{
	int ix,jx,bs_loc,fn_loc;
	if(strlen(pf)==0) {
		if(path) *path=0;
		if(file) *file=0;
		return;
		}
	bs_loc=static_cast<int>(strlen(pf));	// SR DCAST64: Downcast to 2G limit.
	for(ix=bs_loc-1; ix>=0; --ix) {
		if(pf[ix]=='\\')  {
			bs_loc=ix;
			fn_loc=ix+1;
			goto do_split;
			}
		if(pf[ix]==':') {
			bs_loc=ix+1;
			fn_loc=ix+1;
			goto do_split;
			}
		}
	bs_loc= -1;
	fn_loc=0;

	do_split:
	if(file)
		strcpy(file,&pf[fn_loc]);
	if(path) {
		if(bs_loc>0)  {
			for(jx=0; jx<bs_loc; ++jx)
				path[jx]=pf[jx];
			path[jx]=0;
			}
		else  path[0]=0;
		}
	}


//#define AnimaExporterer_CLASS_ID	Class_ID(0x2b2a4b11, 0x3ed9787a)
#define AnimaExporterer_CLASS_ID	Class_ID(0x4a0f2959, 0x5b04071d)

Interface *globalInterface;

class AnimaExportererClassDesc : public ClassDesc {
	public:
	int 			IsPublic() { return 1; }

	void *			Create(BOOL loading = FALSE) { return new AnimaExporter(this); }
	const TCHAR *	ClassName() { return _T("an(i)ma Content Exporter"); }
	SClass_ID		SuperClassID() { return UTILITY_CLASS_ID; }
	Class_ID		ClassID() { return AnimaExporterer_CLASS_ID; }
	const TCHAR* 	Category() { return _T("");  }

	};

static AnimaExportererClassDesc AnimaDesc;

//------------------------------------------------------
// This is the interface to Jaguar:
//------------------------------------------------------
void EnsureInit();

__declspec( dllexport ) const TCHAR *
LibDescription() { return _T("an(i)ma Content Exporter"); }

__declspec( dllexport ) int
LibNumberClasses() { return 1; }


// Return version so can detect obsolete DLLs
__declspec( dllexport ) ULONG 
LibVersion() { return VERSION_3DSMAX; }

// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 0;
}

//typedef ClassDesc* (CALLBACK* MAX_API_AnimaExporterClass)(HINSTANCE hInstance);
//typedef ClassDesc* (CALLBACK* MAX_API_AnimaObjectClass)(HINSTANCE hInstance,const char*programPath);
//typedef ClassDesc* (CALLBACK* MAX_API_AnimaObjectSubMeshClass)(HINSTANCE hInstance);

bool failed=false;

__declspec( dllexport ) ClassDesc *LibClassDesc(int i) {
		//EnsureInit();
		//if (hModule == NULL)
		//{
		//	// avviso
		//	if (!failed)
		//	{
		//		failed=true;
		//	}
		//	return NULL;
		//}
		switch(i) {
			case 0:
				return &AnimaDesc;
				break;
		
			default: return 0; break;
			}
	}

//
// .Anima import module functions follow:
//
//
//AnimaExporter::AnimaExporter() {
//	}
//
//AnimaExporter::~AnimaExporter() {
//	}
//
//int AnimaExporter::ExtCount() {
//	return 1;
//}
//
//const TCHAR * AnimaExporter::Ext(int n) {		// Extensions supported for import/export modules
//	switch(n) {
//		case 0:
//			return _T("anilib");
//		}
//	return _T("");
//	}
//
//const TCHAR * AnimaExporter::LongDesc() {			// Long ASCII description (i.e. "Targa 2.0 Image File")
//	return _T(_T("an(i)ma Content Exporter"));
//	}
//	
//const TCHAR * AnimaExporter::ShortDesc() {			// Short ASCII description (i.e. "Targa")
//	return _T("an(i)ma Content Exporter");
//	}
//
//const TCHAR *AnimaExporter::AuthorName() {			// ASCII Author name
//	return _T("Cecchini Marco");
//	}
//
//const TCHAR *AnimaExporter::CopyrightMessage() {	// ASCII Copyright message
//	return _T("Copyright to AXYZ");
//	}
//
//const TCHAR *AnimaExporter::OtherMessage1() {		// Other message #1
//	return _T("");
//	}
//
//const TCHAR *AnimaExporter::OtherMessage2() {		// Other message #2
//	return _T("");
//	}
//
//unsigned int AnimaExporter::Version() {				// Version number * 100 (i.e. v3.01 = 301)
//	return 100;
//	}
//
//void AnimaExporter::ShowAbout(HWND hWnd) {			// Optional
// 	}

// =============
	
char _norm_programPath[1000];

#define CURRENT_VERSION_64 "SOFTWARE\\Wow6432Node\\aXYZ design\\an(i)ma 1.0"
#define CURRENT_VERSION_32 "SOFTWARE\\aXYZ design\\an(i)ma 1.0"

/** public functions **/
BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {
   if( fdwReason == DLL_PROCESS_ATTACH )
   {
      hInstance = hinstDLL;
    //  DisableThreadLibraryCalls(hInstance);
   }
   return(TRUE);
   }


UtilityObj* AnimaExporter::m_pLEXIMain = NULL;

AnimaExporter::AnimaExporter(AnimaExportererClassDesc* pDesc)
{
//
	//m_pLEXIMain = NULL;
}

AnimaExporter::~AnimaExporter()
{
//
}


//typedef int (CALLBACK* MAX_API_DoExport)(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);

void AnimaExporter::BeginEditParams(Interface* ip, IUtil* iu)
{
	if (hModule != NULL) return;

	 FILE *fp;
	 FILE *configFp = NULL;
	 int len,i;
	 DWORD dwType,dwSize;
	 DWORD dataType;
	 DWORD dataSize;
	 HKEY resultHandle = 0;
	 LONG res;
	
	 char *l;
	 char CurrentPath[1000];
	 char *path;
	 char programPath[10000];
	 char _programPath[10000];
	 char line[1000];
	 //char pathToTry[][1000]={"\\plugcfg\\Anima.xml","\\plugcfg\\Anima\\Anima.xml" };

#ifndef WIN64

	#ifdef MAX9
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.Export.MAX9_d.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.Export.MAX9.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif
	  
	#ifdef MAX2008
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.Export.MAX2008_d.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.Export.MAX2008.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2009
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.Export.MAX2009_d.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.Export.MAX2009.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2010
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.Export.MAX2010_d.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.Export.MAX2010.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2011
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.Export.MAX2011_d.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.Export.MAX2011.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2012
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.Export.MAX2012_d.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.Export.MAX2012.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif
#else

	#ifdef MAX2008
	#ifdef _DEBUG
		 char lwPath[] = {"\\x64\\Anima.Export.MAX2008_d.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#else
		 char lwPath[] = {"\\x64\\Anima.Export.MAX2008_x64.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2009
	#ifdef _DEBUG
		 char lwPath[] = {"\\x64\\Anima.Export.MAX2009_d.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#else
		 char lwPath[] = {"\\x64\\Anima.Export.MAX2009_x64.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	 #ifdef MAX2010
	#ifdef _DEBUG
		 char lwPath[] = {"\\x64\\Anima.Export.MAX2010_d.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#else
		 char lwPath[] = {"\\x64\\Anima.Export.MAX2010_x64.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2011
	#ifdef _DEBUG
		 char lwPath[] = {"\\x64\\Anima.Export.MAX2011_d.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#else
		 char lwPath[] = {"\\x64\\Anima.Export.MAX2011_x64.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2012
	#ifdef _DEBUG
		 char lwPath[] = {"\\x64\\Anima.Export.MAX2012_d.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#else
		 char lwPath[] = {"\\x64\\Anima.Export.MAX2012_x64.dll"}; // BO con {"/Anima.Export.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif
#endif

	dwSize = 9999;

	// prima controllo il file Anima.xml

	//getcwd(CurrentPath, _MAX_PATH);
	memset(programPath,0,10000);
	strcpy(programPath,"");

//	fp = fopen( "c:\\axyz.txt", "w" );
//	fprintf(fp,"CurrentPath %s\n",CurrentPath);

	IPathConfigMgr *mgr =  IPathConfigMgr::GetPathConfigMgr();
	for(int i=0;i<mgr->GetPlugInEntryCount();i++)
	{
		_tcscpy(CurrentPath,mgr->GetPlugInDir(i));

	/*}
	for(i=0;i<2;i++)
	{*/
		strcpy(_programPath,CurrentPath);
		//strcat(_programPath,pathToTry[i]);
		strcat(_programPath,"Anima.xml");
		configFp = fopen(_programPath, "r" );

		//fprintf(fp,"try programPath %s\nfile: %s\n",_programPath,(configFp!=NULL) ?"OPEN":"CLOSE");


		if (configFp!=NULL)
		{
			strcpy(_programPath,"");

		//	fprintf(fp,"aperto\n");

			l=NULL;
			do
			{
				l = fgets(line,1000,configFp);

			//	fprintf(fp,"line: %s\n",line);

				if (strncmp(line,"PATH",4) == 0)
				{
					strcpy(_programPath,line+5);

			//		fprintf(fp,"find programPath: %s\n",_programPath);

					break;
				}
			}while(l!=NULL);

			if (strlen(_programPath) > 0)
			{
				strcpy(programPath,_programPath);

			//	fprintf(fp,"anima.xml programPath: %s\n",programPath);

				// faccio la normalizzazione
				///*len=0;
				//for(i=0;i<strlen(_programPath);i++)
				//{
				//	programPath[len++] = _programPath[i];
				//	programPath[len++] = '\0';
				//}
				//programPath[len++] = '\0';
				//programPath[len++] = '\0';*/
			}

			fclose(configFp);
		}
	}

	//strcpy(programPath,"");
	if (strlen(programPath) == 0)
	{
		//fp = fopen( "c:\\axyz.txt", "w" );
		//trace("Anima","Startup","lw %s",lwPath);

		#ifndef WIN64
		 if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,  TEXT(CURRENT_VERSION_32), 0, KEY_READ, &resultHandle) == ERROR_SUCCESS)
		 {
			 if (RegQueryValueEx(resultHandle,  TEXT("path"), 0, &dataType, 0, &dataSize) ==   ERROR_SUCCESS)
			 {
				if ( RegQueryValueEx(resultHandle, TEXT("path"), 0, 0, (LPBYTE)programPath, &dataSize) == ERROR_SUCCESS)
				{
				}
				//else
					// trace("Anima","Startup","Registry Error ");
			 }
			// else
				 // trace("Anima","Startup","Registry Error ");

			RegCloseKey(resultHandle);
		 }
		 else
		 {
			// trace("Anima","Startup","Registry Error ");
		 }
		#else

		 if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,  TEXT(CURRENT_VERSION_64), 0, KEY_READ, &resultHandle) == ERROR_SUCCESS)
		 {
			 if (RegQueryValueEx(resultHandle,  TEXT("path"), 0, &dataType, 0, &dataSize) ==   ERROR_SUCCESS)
			 {
				if ( RegQueryValueEx(resultHandle, TEXT("path"), 0, 0, (LPBYTE)programPath, &dataSize) == ERROR_SUCCESS)
				{
				}
				//else
					// trace("Anima","Startup","Registry Error ");
			 }
			// else
				 // trace("Anima","Startup","Registry Error ");

			RegCloseKey(resultHandle);
		 }
		 else
		 {
			// trace("Anima","Startup","Registry Error ");
		 }
		#endif WIN64
	}

	//fprintf(fp,"registry programPath: %s\n",programPath); 

	////len = -1;
	////for(i=0;i<1000 && len == -1;i+=2)
	////{
	////	if (programPath[i] == programPath[i+1] && programPath[i] == '\0')
	////		len = i;
	////}

	//for(int i=0,ic=0;i<len;i+=2)
	//	_norm_programPath[ic++] = programPath[i];

	//len=len/2;

	len = strlen(programPath);
	for(int i=0,ic=0;i<len;i++)
		_norm_programPath[ic++] = programPath[i];

	for(i=0;i<strlen(lwPath);i++)
	{
		_norm_programPath[len++] = lwPath[i];
	}

	//trace("Anima","Startup","5 %s",programPath);
	//for(i=0;i<strlen(lwPath);i++)
	//{
	//	programPath[len++] = lwPath[i];
	//	programPath[len++] = '\0';
	//}
	//trace("Anima","Startup","6 %s",programPath);

	_norm_programPath[len++] = '\0';
	//programPath[len++] = '\0';

//	fclose(fp);

//	strcpy(programPath,"C:\\Lavoro\\Anima\\bin\\Anima.MAX2009.dll\0");

	strcpy(anima_programPath ,  _norm_programPath);

	if(m_pLEXIMain==NULL)
	{
		m_hModuleHandle = LoadLibraryEx( TEXT(_norm_programPath),
			//programPath,
			NULL,
			LOAD_WITH_ALTERED_SEARCH_PATH
			); 

		if (m_hModuleHandle == NULL)
		{			
		//	if (!failed)
			{
				char buff[1000];
				strcpy(buff,"Cannot initialize an(i)ma studio engine, please check configuration.\nFound path=");
				strcat(buff,_norm_programPath);
				MessageBox(GetActiveWindow(), TEXT(buff), TEXT("an(i)ma StartupError"), MB_OK);
			}
			return;
		}

		m_fnCreate = (fnCreate)GetProcAddress(m_hModuleHandle, "CreateLEXI");
		m_fnDestroy = (fnDestroy)GetProcAddress(m_hModuleHandle, "DestroyLEXI");

		if(m_fnCreate == NULL || m_fnDestroy == NULL)
		{
			char buff[1000];
			strcpy(buff,"Cannot initialize an(i)ma studio engine, bad API, please check configuration.\nFound path=");
			strcat(buff,_norm_programPath);
			MessageBox(GetActiveWindow(), TEXT(buff), TEXT("an(i)ma StartupError"), MB_OK);

			//MessageBox(NULL,NDS_EXPORTER_PLUGIN,"Error Loading Dll:",MB_ICONERROR);
			FreeLibrary(m_hModuleHandle);
		//	SetCurrentDirectory(curDir);
			return;
		}

		m_pLEXIMain = m_fnCreate();
	}
	
	m_pLEXIMain->BeginEditParams(ip,iu);
	// Set a global prompt display switch
	/*showPrompts = suppressPrompts ? FALSE : TRUE;

	EnsureInit();

	if (hModule == NULL) return IMPEXP_FAIL;

	MAX_API_DoImport p_do_import = (MAX_API_DoImport)GetProcAddress(hModule,"MAX_API_DoImport");
	return p_do_import(anima_programPath,filename,in,gi,suppressPrompts);
	return 0;*/
}

void AnimaExporter::EndEditParams(Interface* ip, IUtil* iu)
{
//	
	if(m_pLEXIMain==NULL)
		return;

	m_pLEXIMain->EndEditParams(ip,iu);

#if(NDS_EXPORTER_AUTOUNLOAD)	
	m_fnDestroy(m_pLEXIMain);
	FreeLibrary(m_hModuleHandle);
	m_pLEXIMain = NULL;
#endif
}

void AnimaExporter::DeleteThis()
{
	delete this;
}