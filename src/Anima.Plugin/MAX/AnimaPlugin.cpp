#include "Max.h"
#include <stdio.h>
#include <direct.h>
#include <commdlg.h>
//#include "AnimaImp.h"
#include "iparamb2.h"
#include "iparamm2.h"

#include <maxheapdirect.h>

HINSTANCE hInstance;

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
//
//static int MessageBox2(int s1, int s2, int option = MB_OK) {
//	TSTR str1(GetString(s1));
//	TSTR str2(GetString(s2));
//	return MessageBox(GetActiveWindow(), str1, str2, option);
//	}
//
//static int MessageBox(TCHAR *s, int s2, int option = MB_OK) {
//	TSTR str1(s);
//	TSTR str2(GetString(s2));
//	return MessageBox(GetActiveWindow(), str1, str2, option);
//	}
//

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



//------------------------------------------------------
// This is the interface to Jaguar:
//------------------------------------------------------

__declspec( dllexport ) const TCHAR *
LibDescription() { return _T(".ani Anima (AXYZ)"); }

__declspec( dllexport ) int
LibNumberClasses() { return 2; }

// Return version so can detect obsolete DLLs
__declspec( dllexport ) ULONG 
LibVersion() { return VERSION_3DSMAX; }

// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 1;
}
// =============

HMODULE hModule = NULL;

void EnsureInit() {

	if (hModule != NULL) return;

	 int len,i;
	 DWORD dwType,dwSize;
	 DWORD dataType;
	 DWORD dataSize;
	 HKEY resultHandle = 0;
	 LONG res;

	 char programPath[10000];
#ifdef _DEBUG
	 char lwPath[] = {"\\Anima_d.MAX.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
#else
	  char lwPath[] = {"\\Anima.MAX.dll"}; // BO con {"/Anima.MAX.dll"}; carica le qt da unl'altra parte
#endif
//
//	 char lwPath[] = {"\\Anima.LW.dll"};
////
	dwSize = 9999;

	//fp = fopen( "c:\\axyz.txt", "w" );
	//trace("Anima","Startup","lw %s",lwPath);

     if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,  TEXT("SOFTWARE\\Anima\\Path"), 0, KEY_READ, &resultHandle) == ERROR_SUCCESS)
	 {
		 if (RegQueryValueEx(resultHandle,  TEXT("programPath"), 0, &dataType, 0, &dataSize) ==   ERROR_SUCCESS)
		 {
			if ( RegQueryValueEx(resultHandle, TEXT("programPath"), 0, 0, (LPBYTE)programPath, &dataSize) == ERROR_SUCCESS)
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

	len = -1;
	for(i=0;i<1000;i+=2)
	{
		if (programPath[i] == programPath[i+1] && programPath[i] == '\0')
			len = i;
	}

	//trace("Anima","Startup","5 %s",programPath);

	for(i=0;i<strlen(lwPath);i++)
	{
		programPath[len++] = lwPath[i];
		programPath[len++] = '\0';
	}
	//trace("Anima","Startup","6 %s",programPath);

	programPath[len++] = '\0';
	programPath[len++] = '\0';

	hModule = LoadLibraryEx(
			(LPCWSTR)programPath,
			NULL,
			LOAD_WITH_ALTERED_SEARCH_PATH
			); 

//	fclose(fp);
	//return fp;
}

/** public functions **/
BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {
   if( fdwReason == DLL_PROCESS_ATTACH )
   {
      hInstance = hinstDLL;
      DisableThreadLibraryCalls(hInstance);
   }
   return(TRUE);
   }

//typedef int (CALLBACK* MAX_API_DoImport)(MeshObjectClassDesc *objDesc,const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts);

/*
int AnimaImport::DoImport(const TCHAR *filename,ImpInterface *in,Interface *gi, BOOL suppressPrompts) {
	// Set a global prompt display switch
	showPrompts = suppressPrompts ? FALSE : TRUE;

	EnsureInit();

	if (hModule == NULL) return IMPEXP_FAIL;

	MAX_API_DoImport p_do_import = (MAX_API_DoImport)GetProcAddress(hModule,"MAX_API_DoImport");
	return p_do_import(&meshDesc,filename,in,gi,suppressPrompts);
}

typedef void * (CALLBACK* MAX_API_CreateObject)(MeshObjectClassDesc *objDesc,BOOL loading);

void *MeshObjectClassDesc::Create(BOOL loading )
{
	EnsureInit();

	if (hModule == NULL) return NULL;

	MAX_API_CreateObject createObj = (MAX_API_CreateObject)GetProcAddress(hModule,"MAX_API_CreateObject");
	return createObj(&meshDesc,loading);
}

typedef HINSTANCE (CALLBACK* MAX_API_GetHINSTANCE)(MeshObjectClassDesc *objDesc);

HINSTANCE MeshObjectClassDesc::HInstance()	
{
	return hInstance;
	//if (hModule == NULL) return NULL;
	//MAX_API_GetHINSTANCE getObj = (MAX_API_GetHINSTANCE)GetProcAddress(hModule,"MAX_API_GetHINSTANCE");
	//return getObj(&meshDesc);
}
HINSTANCE AnimaClassDesc::HInstance()	
{
	return hInstance;
}
*/

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

typedef ClassDesc* (CALLBACK* MAX_API_AnimaImportClass)(HINSTANCE hInstance);
typedef ClassDesc* (CALLBACK* MAX_API_AnimaObjectClass)(HINSTANCE hInstance);

__declspec( dllexport ) ClassDesc *
LibClassDesc(int i) {
	EnsureInit();
	if (hModule == NULL) return NULL;
	ClassDesc *desc;
	MAX_API_AnimaImportClass call1;
	MAX_API_AnimaObjectClass call2;

	switch(i) {
		case 0:
			call1 = (MAX_API_AnimaImportClass)GetProcAddress(hModule,"MAX_API_AnimaImportClass");
			desc = call1(hInstance); 
			return desc;
			break;
		case 1: 
			call2 = (MAX_API_AnimaObjectClass)GetProcAddress(hModule,"MAX_API_AnimaObjectClass");
			desc = call2(hInstance); 
			return desc;
			break;
		default: return 0; break;
		}
	}
