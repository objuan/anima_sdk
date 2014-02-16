// This is a wrapper to load c4d plugins from a different directory.

// You then can load plugins from paths that are not within the c4d\plugins directory
// The res folder must be present at the wrapper location, that's all!

#ifdef __PC
	#include <windows.h>
#endif

#include "c4d_plugin.h"
#include "c4d_file.h"
#include "c4d_resource.h"
//
//#ifdef __MAC
//	#ifdef __cplusplus
//	extern "C" {
//	#endif
//
//	#define C4D_MAIN LONG c4d_main(LONG action, void* p1, void* p2, void* p3)
//	C4D_MAIN;
//
//	#ifdef __cplusplus
//	}
//	#endif
//#elif defined __PC
//  HINSTANCE g_hinstDLL;
//  LPVOID g_lpReserved;
//
//	#ifdef __cplusplus
//	extern "C" {
//	#endif
//
//	#define C4D_MAIN LONG __declspec(dllexport) c4d_main(LONG action, void* p1, void* p2, void* p3)
//	C4D_MAIN;
//
//	#ifndef __WINCRTINIT
//	BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
//	{
//		switch (ul_reason_for_call)
//		{
//			case DLL_PROCESS_ATTACH:
//				g_hinstDLL = (HINSTANCE)hModule;
//				g_lpReserved = lpReserved;
//			//case DLL_THREAD_ATTACH:
//			//case DLL_THREAD_DETACH:
//			//case DLL_PROCESS_DETACH:
//				break;
//		}
//	  return TRUE;
//	}
//
//	#else
//  extern BOOL APIENTRY _CRT_INIT(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);
//
//  BOOL APIENTRY DllMainCRTStartup( HINSTANCE hinst, DWORD reason, LPVOID reserved )
//	{
//		g_hinstDLL = hinst;
//		g_lpReserved = reserved;
//		return TRUE;
//	}
//	#endif
//	#ifdef __cplusplus
//	}
//	#endif
//#else
//	#ifdef __cplusplus
//	extern "C" {
//	#endif
//
//	#define C4D_MAIN __attribute__ ((visibility("default"))) LONG c4d_main(LONG action, void* p1, void* p2, void* p3)
//	C4D_MAIN;
//
//	#ifdef __cplusplus
//	}
//	#endif
//#endif
//
//Int32 InitOS(void *p);

Filename *path_storage;

#include <stdio.h>

HMODULE hModule = NULL;
typedef LONG (*C4DMAINMETHOD)(LONG, void*, void*, void*);
bool EnsureInit()
{
	if(!hModule)
	{
		CHAR* cdlpath = "Anima.C4D10";
#if API_VERSION >= 10000
		cdlpath = "Anima.C4D10";
#endif
#if API_VERSION >= 11000
		cdlpath = "Anima.C4D11";
#endif
#if API_VERSION >= 11500
		cdlpath = "Anima.C4D115";
#endif
#if API_VERSION >= 12000
		cdlpath = "Anima.C4D12";
#endif
#if API_VERSION >= 13000
		cdlpath = "Anima.C4D13";
#endif
#if API_VERSION >= 15000
		cdlpath = "Anima.C4D15";
#endif
		CHAR* extension = ".cdl";
#ifdef __C4D_64BIT
		extension = ".cdl64";
#endif

		LPTSTR  thisCdlPath = new TCHAR[_MAX_PATH];
		//GetModuleFileName(g_hinstDLL, thisCdlPath, _MAX_PATH);
#ifndef __API_INTERN__
	 Filename f = GeGetPluginPath();
#else
	  Filename f =GeGetStartupPath();
#endif
	
		CHAR configPath[_MAX_PATH];
		CHAR* dirpos = strrchr(thisCdlPath, '\\');
		strncpy(configPath, thisCdlPath, dirpos-thisCdlPath+1);
		configPath[dirpos-thisCdlPath+1] = '\0';
#ifdef __C4D_64BIT
		strcat(configPath, "Anima64x.xml");
#else
		strcat(configPath, "Anima.xml");
#endif

		printf(configPath);
		delete thisCdlPath;
		FILE* configFp = fopen(configPath, "r" );
		char *l;
		char line[1000];
		CHAR animaPath[_MAX_PATH];
		if(!configFp)
		{
			MessageBox(GetActiveWindow(), TEXT("Anima.xml File not found!"), TEXT("an(i)ma StartupError"), MB_OK);
			printf("Anima.xml File not found!\n");
			return false;
		}
		do
		{
			l = fgets(line,1000,configFp);
			if (strncmp(line,"PATH",4) == 0)
			{
				strcpy(animaPath, line+5);

				printf("found programPath %s\n",animaPath);

				break;
			}
		}while(l!=NULL);
		fclose(configFp);
		strcat(animaPath, "\\");
#ifdef __C4D_64BIT
		strcat(animaPath, "x64\\");
#endif
		strcat(animaPath, cdlpath);
		strcat(animaPath, extension);

//		hModule = LoadLibrary( TEXT("C:\\work\\Cinema_Demos\\12.0\\plugins_\\pluginexport\\pluginexport.cdl")); 
//		hModule = LoadLibraryEx( TEXT("C:\\work\\aXYZ\\bin\\Anima.C4D12.cdl"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH); 
		hModule = LoadLibraryEx( animaPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH); 
		if(!hModule)
		{
			MessageBox(GetActiveWindow(), TEXT("Can't load anima dlls"), TEXT("an(i)ma StartupError"), MB_OK);
			MessageBox(GetActiveWindow(), TEXT(animaPath), TEXT("an(i)ma StartupError"), MB_OK);
		}
	}
	return true;
}
//
//C4D_MAIN //(LONG action, void* p1, void* p2, void* p3)
//{
//	EnsureInit();
//	C4DMAINMETHOD call = (C4DMAINMETHOD)GetProcAddress(hModule,"c4d_main");
//	if(call)
//		return call(action, p1, p2, p3);
//	else
//		return FALSE;
//}

Bool PluginStart(void)
{
	return true;
}

void PluginEnd(void)
{
	
}

Bool PluginMessage(Int32 id, void* data)
{
	switch (id)
	{
		case C4DPL_INIT_SYS:
			if (!resource.Init())
				return false;		// don't start plugin without resource

			// register example datatype. This is happening at the earliest possible time
			/*if (!RegisterExampleDataType())
				return false;*/
			if (!EnsureInit())
				return false;

			// serial hook example; if used must be registered before PluginStart(), best in C4DPL_INIT_SYS
			//if (!RegisterExampleSNHook()) return false;

			return true;

		case C4DMSG_PRIORITY:
			//react to this message to set a plugin priority (to determine in which order plugins are initialized or loaded
			//SetPluginPriority(data, mypriority);
			return true;

		case C4DPL_BUILDMENU:
			//react to this message to dynamically enhance the menu
			//EnhanceMainMenu();
			break;

		case C4DPL_COMMANDLINEARGS:
			//sample implementation of command line rendering:
			//void CommandLineRendering(C4DPL_CommandLineArgs* args);
			//CommandLineRendering((C4DPL_CommandLineArgs*)data);

			//react to this message to react to command line arguments on startup
			/*
			{
				C4DPL_CommandLineArgs *args = (C4DPL_CommandLineArgs*)data;
				Int32 i;

				for (i=0;i<args->argc;i++)
				{
					if (!args->argv[i]) continue;

					if (!strcmp(args->argv[i],"--help") || !strcmp(args->argv[i],"-help"))
					{
						// do not clear the entry so that other plugins can make their output!!!
						GePrint("\x01-SDK is here :-)");
					}
					else if (!strcmp(args->argv[i],"-SDK"))
					{
						args->argv[i] = nullptr;
						GePrint("\x01-SDK executed:-)");
					}
					else if (!strcmp(args->argv[i],"-plugincrash"))
					{
						args->argv[i] = nullptr;
						*((Int32*)0) = 1234;
					}
				}
			}
			*/
			break;

		case C4DPL_EDITIMAGE:
			/*{
				C4DPL_EditImage *editimage = (C4DPL_EditImage*)data;
				if (!data) break;
				if (editimage->return_processed) break;
				GePrint("C4DSDK - Edit Image Hook: "+editimage->imagefn->GetString());
				// editimage->return_processed = true; if image was processed
			}*/
			return false;
	}

	return false;
}
