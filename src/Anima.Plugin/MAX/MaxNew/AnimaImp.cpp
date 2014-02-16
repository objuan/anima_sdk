#include "Max.h"
#include <stdio.h>
#include <direct.h>
#include <commdlg.h>
#include "AnimaImp.h"
//#include "iparamb2.h"
//#include "iparamm2.h"
#include <IPathConfigMgr.h> 

#include <maxheapdirect.h>

HINSTANCE hInstance;
HMODULE hModule = NULL;

static BOOL showPrompts;
//
//TCHAR *GetString(int id)
//	{
//	static TCHAR buf[256];
//	if (hInstance)
//		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
//	return NULL;
//	}
//
//static int MessageBox(int s1, int s2, int option = MB_OK) {
//	TSTR str1(GetString(s1));
//	TSTR str2(GetString(s2));
//	return MessageBox(GetActiveWindow(), str1, str2, option);
//	}

//static int MessageBox2(int s1, int s2, int option = MB_OK) {
//	TSTR str1(GetString(s1));
//	TSTR str2(GetString(s2));
//	return MessageBox(GetActiveWindow(), str1, str2, option);
//	}
////
//static int MessageBox(TCHAR *s, int s2, int option = MB_OK) {
//	TSTR str1(s);
//	TSTR str2(GetString(s2));
//	return MessageBox(GetActiveWindow(), str1, str2, option);
//	}

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


//void fin_degammify(Color *col, Color *gamcol) {
//	if (gammaMgr.enable) {
//		col->r = deGammaCorrect(gamcol->r, gammaMgr.fileInGamma);
//		col->g = deGammaCorrect(gamcol->g, gammaMgr.fileInGamma);
//		col->b = deGammaCorrect(gamcol->b, gammaMgr.fileInGamma);
//		}
//	else *col = *gamcol;
//	}

//static Color ColorFrom24(Color_24 c) {
//	Color a;
//	a.r = (float)c.r/255.0f;
//	a.g = (float)c.g/255.0f;
//	a.b = (float)c.b/255.0f;
//	return a;
//	}

#define Anima_CLASS_ID	Class_ID(0x7f5810de, 0xe3a308c)

Interface *globalInterface;

class AnimaClassDesc : public ClassDesc {
	public:
	int 			IsPublic() { return 1; }
	//BOOL OkToCreate (Interface *i)
	//{
	//	globalInterface=i; return TRUE;
	//}
	void *			Create(BOOL loading = FALSE) { return new AnimaImport; }
	const TCHAR *	ClassName() { return _T("AnimaImport"); }
	SClass_ID		SuperClassID() { return SCENE_IMPORT_CLASS_ID; }
	Class_ID		ClassID() { return Anima_CLASS_ID; }
	const TCHAR* 	Category() { return _T("Anima Import");  }

#ifdef MAX2014
	virtual const MCHAR * GetRsrcString (int id) {return _M("");}
#else 
	virtual  MCHAR * GetRsrcString (int id) {return _M("");}
#endif
	//virtual FPInterface* __thiscall GetInterface(TCHAR* in){ return NULL; }
	//const TCHAR*	InternalName()				{ return _T("AnimaImport"); }
	//HINSTANCE		HInstance();
	};

static AnimaClassDesc AnimaDesc;

//#define MeshObject_CLASS_ID	Class_ID(0x37240d5, 0x12fb4809)

//------------------------------------------------------
// This is the interface to Jaguar:
//------------------------------------------------------
void EnsureInit();

__declspec( dllexport ) const TCHAR *
LibDescription() { return _T(".ani Anima (AXYZ)"); }

__declspec( dllexport ) int
LibNumberClasses() { return 3; }


// Return version so can detect obsolete DLLs
__declspec( dllexport ) ULONG 
LibVersion() { return VERSION_3DSMAX; }

// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 0;

}

typedef ClassDesc* (CALLBACK* MAX_API_AnimaImportClass)(HINSTANCE hInstance);
typedef ClassDesc* (CALLBACK* MAX_API_AnimaObjectClass)(HINSTANCE hInstance,const char*programPath);
typedef ClassDesc* (CALLBACK* MAX_API_AnimaObjectSubMeshClass)(HINSTANCE hInstance);

bool failed=false;

__declspec( dllexport ) ClassDesc *LibClassDesc(int i) {
		EnsureInit();
		if (hModule == NULL)
		{
			// avviso
			if (!failed)
			{
				failed=true;
			}
			return NULL;
		}
		ClassDesc *desc;
		ClassDesc *desc2;
		MAX_API_AnimaImportClass call1;
		MAX_API_AnimaObjectClass call2;
		MAX_API_AnimaObjectSubMeshClass call3;
		//MAX_API_AnimaObjectSubMeshClass call4;
		//MAX_API_AnimaObjectSubMeshClass call5;

		switch(i) {
			case 0:
			/*	call1 = (MAX_API_AnimaImportClass)GetProcAddress(hModule,"MAX_API_AnimaImportClass");
				desc = call1(hInstance); 
				return desc;*/
				return &AnimaDesc;
				break;
			case 1: 
				call2 = (MAX_API_AnimaObjectClass)GetProcAddress(hModule,"MAX_API_AnimaObjectClass");
				desc = call2(hInstance,anima_programPath); 
				return desc;
				break;
			case 2: 
				call3 = (MAX_API_AnimaObjectSubMeshClass)GetProcAddress(hModule,"MAX_API_AnimaObjectSubMeshClass");
				desc2 = call3(hInstance); 
				return desc2;
				break;
		/*	case 3: 
				call4 = (MAX_API_AnimaObjectSubMeshClass)GetProcAddress(hModule,"MAX_API_AnimaObjectDummyClass");
				desc2 = call4(hInstance); 
				return desc2;
				break;
			case 4:
				call5 = (MAX_API_AnimaObjectSubMeshClass)GetProcAddress(hModule,"MAX_API_AnimaObjectGeomClass");
				desc2 = call5(hInstance); 
				return desc2;
				break;*/
			default: return 0; break;
			}
	}

//
// .Anima import module functions follow:
//

AnimaImport::AnimaImport() {
	}

AnimaImport::~AnimaImport() {
	}

int AnimaImport::ExtCount() {
	return 1;
}

const TCHAR * AnimaImport::Ext(int n) {		// Extensions supported for import/export modules
	switch(n) {
		case 0:
			return _T("ANI");
		}
	return _T("");
	}

const TCHAR * AnimaImport::LongDesc() {			// Long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("Anima format file");
	}
	
const TCHAR * AnimaImport::ShortDesc() {			// Short ASCII description (i.e. "Targa")
	return _T("Anima");
	}

const TCHAR *AnimaImport::AuthorName() {			// ASCII Author name
	return _T("AXYZ");
	}

const TCHAR *AnimaImport::CopyrightMessage() {	// ASCII Copyright message
	return _T("Copyright to AXYZ");
	}

const TCHAR *AnimaImport::OtherMessage1() {		// Other message #1
	return _T("");
	}

const TCHAR *AnimaImport::OtherMessage2() {		// Other message #2
	return _T("");
	}

unsigned int AnimaImport::Version() {				// Version number * 100 (i.e. v3.01 = 301)
	return 100;
	}

void AnimaImport::ShowAbout(HWND hWnd) {			// Optional
 	}

// =============
	
char _norm_programPath[1000];

#define CURRENT_VERSION_64 "SOFTWARE\\Wow6432Node\\aXYZ design\\an(i)ma 1.0"
//#define CURRENT_VERSION_32 "SOFTWARE\\aXYZ design\\an(i)ma 1.0"
#define CURRENT_VERSION_32 "SOFTWARE\\aXYZ design\\an(i)ma 1.0"

void EnsureInit() {

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
	  char buffer[1000];
	 //char pathToTry[][1000]={"\\plugcfg\\Anima.xml","\\plugcfg\\Anima\\Anima.xml" };

#ifndef WIN64

	#ifdef MAX9
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.MAX9_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.MAX9.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif
	  
	#ifdef MAX2008
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.MAX2008_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.MAX2008.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2009
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.MAX2009_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.MAX2009.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2010
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.MAX2010_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.MAX2010.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2011
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.MAX2011_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.MAX2011.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2012
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.MAX2012_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.MAX2012.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2013
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.MAX2013_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.MAX2013.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2014
	#ifdef _DEBUG
		 char lwPath[] = {"\\Anima.MAX2014_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		  char lwPath[] = {"\\Anima.MAX2014.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif
#else

	#ifdef MAX2008
	#ifdef _DEBUG
		 char lwPath[] = {"\\x64\\Anima.MAX2008_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		 char lwPath[] = {"\\x64\\Anima.MAX2008_x64.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2009
	#ifdef _DEBUG
		 char lwPath[] = {"\\x64\\Anima.MAX2009_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		 char lwPath[] = {"\\x64\\Anima.MAX2009_x64.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	 #ifdef MAX2010
	#ifdef _DEBUG
		 char lwPath[] = {"\\x64\\Anima.MAX2010_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		 char lwPath[] = {"\\x64\\Anima.MAX2010_x64.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2011
	#ifdef _DEBUG
		 char lwPath[] = {"\\x64\\Anima.MAX2011_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		 char lwPath[] = {"\\x64\\Anima.MAX2011_x64.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2012
	#ifdef _DEBUG
		 char lwPath[] = {"\\x64\\Anima.MAX2012_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		 char lwPath[] = {"\\x64\\Anima.MAX2012_x64.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	#ifdef MAX2013
	#ifdef _DEBUG
		 char lwPath[] = {"\\x64\\Anima.MAX2013_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		 char lwPath[] = {"\\x64\\Anima.MAX2013_x64.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif

	 #ifdef MAX2014
	#ifdef _DEBUG
		 char lwPath[] = {"\\x64\\Anima.MAX2014_d.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#else
		 char lwPath[] = {"\\x64\\Anima.MAX2014_x64.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
	#endif
	#endif
#endif


//
//	 char lwPath[] = {"\\Anima.LW.dll"};
////

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
		size_t s =  wcstombs  ( buffer,mgr->GetPlugInDir(i) , wcslen(mgr->GetPlugInDir(i)));
		buffer[s] = '\0';
		strcpy(CurrentPath,buffer);

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

	 wchar_t qpath[2000];
	size_t s =  mbstowcs  ( qpath,anima_programPath , strlen(anima_programPath)+1);
	//size_t mbstowcs ( wchar_t * wcstr, const char * mbstr, size_t max );
	//hModule = LoadLibraryEx( _M("C:\\Lavoro\\anima\\trunk\\bin\\Anima.MAX2013.dll"),
	hModule = LoadLibraryEx(qpath,
			//programPath,
			NULL,
			LOAD_WITH_ALTERED_SEARCH_PATH
			); 

	if (hModule == NULL)
	{			
		if (!failed)
		{
//				//MessageBox(GetActiveWindow(), (LPCWSTR)L"Cannot initialize anima engine, please check configuration", (LPCWSTR)L"Anima Message", MB_OK);
//#ifdef WIN64
			char buff[1000];
			strcpy(buff,"Cannot initialize an(i)ma engine, please check configuration.\nFound path=");
			strcat(buff,_norm_programPath);

			 wchar_t aa[2000];
			size_t s =  mbstowcs  ( aa,buff , strlen(buff)+1);

			MessageBox(GetActiveWindow(), aa, _M("an(i)ma StartupError"), MB_OK);
//#endif
		}
	}
	
	//fclose(fp);
	//return fp;
}

/** public functions **/
BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {
   if( fdwReason == DLL_PROCESS_ATTACH )
   {
      hInstance = hinstDLL;
    //  DisableThreadLibraryCalls(hInstance);
   }
   return(TRUE);
   }


typedef int (CALLBACK* MAX_API_DoImport)(const char *programPath,const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts);

int AnimaImport::DoImport(const TCHAR *filename,ImpInterface *in,Interface *gi, BOOL suppressPrompts) {
	// Set a global prompt display switch
	showPrompts = suppressPrompts ? FALSE : TRUE;

	EnsureInit();

	if (hModule == NULL) return IMPEXP_FAIL;

	MAX_API_DoImport p_do_import = (MAX_API_DoImport)GetProcAddress(hModule,"MAX_API_DoImport");
	return p_do_import(anima_programPath,filename,in,gi,suppressPrompts);
	return 0;
}

//typedef void * (CALLBACK* MAX_API_CreateObject)(MeshObjectClassDesc *objDesc,BOOL loading);
//
//void *MeshObjectClassDesc::Create(BOOL loading )
//{
//	EnsureInit();
//
//	if (hModule == NULL) return NULL;
//
//	MAX_API_CreateObject createObj = (MAX_API_CreateObject)GetProcAddress(hModule,"MAX_API_CreateObject");
//	return createObj(&meshDesc,loading);
//}
//
//typedef HINSTANCE (CALLBACK* MAX_API_GetHINSTANCE)(MeshObjectClassDesc *objDesc);
//
//HINSTANCE MeshObjectClassDesc::HInstance()	
//{
//	return hInstance;
//	//if (hModule == NULL) return NULL;
//	//MAX_API_GetHINSTANCE getObj = (MAX_API_GetHINSTANCE)GetProcAddress(hModule,"MAX_API_GetHINSTANCE");
//	//return getObj(&meshDesc);
//}
//HINSTANCE AnimaClassDesc::HInstance()	
//{
//	return hInstance;
//}

//typedef void * (CALLBACK* MAX_API_CreateBlobObject)(BlobMeshClassDesc *objDesc,BOOL loading);
//
//void *BlobMeshClassDesc::Create(BOOL loading )
//{
//	EnsureInit();
//
//	if (hModule == NULL) return NULL;
//
//	MAX_API_CreateBlobObject createObj = (MAX_API_CreateBlobObject)GetProcAddress(hModule,"MAX_API_CreateBlobObject");
//	return createObj(&blobDesc,loading);
//}
//

