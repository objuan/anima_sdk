/*
*/
#include <windows.h>
#include <winreg.h>
#include <lwanimlod.h>
#include <lwanimsav.h>
#include <lwchannel.h>
#include <lwdisplce.h>
#include <lwenviron.h>
#include <lwframbuf.h>
#include <lwfilter.h>
#include <lwmotion.h>
#include <lwmaster.h>
#include <lwobjrep.h>
#include <lwtexture.h>
#include <lwshader.h>
#include <lwvolume.h>
#include <stdarg.h>
#include <stdlib.h>


//#include <lwcustobj.h>
#include <lwobjimp.h>

#include "axyz.h"

static FILE *fp;
HMODULE hModule = NULL;
char out_programPath[1000];
char _norm_programPath[1000];
void trace( const char *func, const char *plug, const char *fmt, ... )
{
   if ( fp ) {
      if ( func )  fprintf( fp, "%-22.22s", func );
      if ( plug )  fprintf( fp, "%-23.23s", plug  );
      if ( fmt ) {
         va_list ap;
         va_start( ap, fmt );
         vfprintf( fp, fmt, ap );
         va_end( ap );
      }
      fprintf( fp, "\n" );
	  fflush(fp);
   }
}

/*
char *SearchDirectory(const char *CurrentPath,const char *refcstrRootDirectory)
{
   HANDLE hFile; // Handle to file
 //  WIN32_FIND_DATA FileInformation; // File information
   LPWIN32_FIND_DATA FileInformation;
   //LPWIN32_FIND_DATA FileInformation = new WIN32_FIND_DATA;
   const char strPattern[1000];
   const char strFilePath[1000];
    LPWSTR pBuffer = NULL;
    LPWSTR pMessage = L"%1!*.*s! %3 %4!*s!";

   strcpy(strPattern,CurrentPath);
   strcat(strPattern, refcstrRootDirectory);
  // strcat(strPattern, "\\*.*");
     
   hFile = FindFirstFile(CurrentPath, &FileInformation);
   if(hFile != INVALID_HANDLE_VALUE)
   {
    //  do
    //  {
    //     if(FileInformation.cFileName[0] != '.')
    //     {
			 //strcpy(strFilePath,refcstrRootDirectory);
			 //strcat(refcstrRootDirectory, "\\");
			 //strcat(refcstrRootDirectory, FileInformation.cFileName);

    //        if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    //        {
    //              // Search subdirectory
    //              int iRC = SearchDirectory(CurrentPath, strFilePath);
    //              if(iRC)
    //                 return iRC;
    //        }
    //        else
    //        {
				//if (strcmp( FileInformation.cFileName,"Anima.xml") == 0)
				//{
				//	 FindClose(hFile);
				//     return strFilePath;
				//}
    //        }
    //     }
    //  } while(FindNextFile(hFile, &FileInformation) == TRUE);
    //  
    //  // Close handle
    //  FindClose(hFile);

      return NULL;
   }
   return NULL;
}
*/
void *Startup( void )
{

	 int len,i,ic;
	 DWORD dwType,dwSize;
	 DWORD dataType;
	 DWORD dataSize;
	 HKEY resultHandle = 0;
	 LONG res;
	 FILE *configFp = NULL;

	 char *l;
	 char CurrentPath[1000];
	 char *path;
	 char programPath[10000];
	 char _programPath[10000];
	 char line[1000];
	 char pathToTry[][1000]={"\\..\\Plugins\\Anima.xml","\\..\\Plugins\\Anima\\Anima.xml" };

#ifdef _DEBUG
	 char lwPath[] = {"\\Anima_d.LW.dll"}; // BO con {"/Anima.LW.dll"}; carica le qt da unl'altra parte
#else
	  char lwPath[] = {"\\Anima.LW.dll"}; // BO con {"/Anima.LW.dll"}; carica le qt da unl'altra parte
#endif
//
//	 char lwPath[] = {"\\Anima.LW.dll"};
////
	dwSize = 9999;

	// prima controllo il file Anima.xml

	getcwd(CurrentPath, _MAX_PATH);
	strcpy(programPath,"");

	for(i=0;i<2;i++)
	{
		strcpy(_programPath,CurrentPath);
		strcat(_programPath,pathToTry[i]);
		configFp = fopen(_programPath, "r" );
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
				// faccio la normalizzazione
				strcpy(programPath,_programPath);
			}

			fclose(configFp);
		}
	}
	// guardo nei registri della macchina

	//fp = fopen( "c:\\axyz.txt", "w" );
	//trace("Anima","Startup","lw %s",lwPath);

	//if (strlen(programPath) == 0)
	//{
	//	 if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,  TEXT("SOFTWARE\\Anima\\Path"), 0, KEY_READ, &resultHandle) == ERROR_SUCCESS)
	//	 {
	//		//trace("Anima","Startup","1");

	//		 if (RegQueryValueEx(resultHandle,  TEXT("programPath"), 0, &dataType, 0, &dataSize) ==   ERROR_SUCCESS)
	//		 {
	//				// Error!
	//			//  trace("Anima","Startup","2");

	//			if ( RegQueryValueEx(resultHandle, TEXT("programPath"), 0, 0, programPath, &dataSize) == ERROR_SUCCESS)
	//			{
	//				//trace("Anima","Startup","3");

	//			}
	//			//else
	//				// trace("Anima","Startup","Registry Error ");

	//		 }
	//		// else
	//			 // trace("Anima","Startup","Registry Error ");

	//		RegCloseKey(resultHandle);
	//	 }
	//	 else
	//	 {
	//		// trace("Anima","Startup","Registry Error ");
	//	 }

	////	 if (RegGetValue(HKEY_CURRENT_USER,
	////              TEXT("SOFTWARE\\Anima\\Path"),
	////              TEXT("programPath"),
	////			   RRF_RT_ANY,
	////              &dwType,
	////              programPath,
	////              &dwSize) != ERROR_SUCCESS)
	////    {
	////        // Error!
	////    }

	//////	strcat(programPath,"\\Anima.LW.dll");

	//	// trace("Anima","Startup","4 %s",programPath);
	//}

	//len = -1;
	//for(i=0;i<1000;i+=2)
	//{
	//	if (programPath[i] == programPath[i+1] && programPath[i] == '\0')
	//		len = i;
	//}

	////trace("Anima","Startup","5 %s",programPath);

	//for(i=0;i<strlen(lwPath);i++)
	//{
	//	programPath[len++] = lwPath[i];
	//	programPath[len++] = '\0';
	//}
	////trace("Anima","Startup","6 %s",programPath);

	//programPath[len++] = '\0';
	//programPath[len++] = '\0';

	len = strlen(programPath);
	for( i=0,ic=0;i<len;i++)
		_norm_programPath[ic++] = programPath[i];

	for(i=0;i<strlen(lwPath);i++)
	{
		_norm_programPath[len++] = lwPath[i];
	}

//	fclose(fp);

	//return fp;
  	
	strcpy(out_programPath,_norm_programPath);

	for( i=0,ic=0;i<len;i++)
	{
		programPath[ic++] = _norm_programPath[i];
		programPath[ic++] = '\0';
	}
	programPath[ic++] = '\0';
	programPath[ic++] = '\0';

	hModule = LoadLibraryEx(
			programPath,
			NULL,
			LOAD_WITH_ALTERED_SEARCH_PATH
			); 

//	if (hModule == NULL)
//	{			
//		//if (!failed)
//		{
////				//MessageBox(GetActiveWindow(), (LPCWSTR)L"Cannot initialize anima engine, please check configuration", (LPCWSTR)L"Anima Message", MB_OK);
////#ifdef WIN64
//			char buff[1000];
//			strcpy(buff,"Cannot initialize an(i)ma engine, please check configuration.\nFound path=");
//			strcat(buff,programPath);
//			MessageBox(GetActiveWindow(), TEXT(buff), TEXT("an(i)ma StartupError"), MB_OK);
////#endif
//		}
//	}

	return (void *) 4;
	//fp = fopen( "c:\\axyz.txt", "w" );
	////fprintf(fp,"Startup");
	////fflush(fp);
	//return fp;
}


void Shutdown( void *serverData )
{
	//if ( fp ) fclose( fp );
}


/* prototypes for the activation functions */
/*
XCALL_( int ) LimitShadow_Handler( long version, GlobalFunc *global, LWShaderHandler *local, void *serverData );
XCALL_( int ) Blotch_Handler( long version, GlobalFunc *global, LWShaderHandler *local, void *serverData );
XCALL_( int ) Blotch_Interface( long version, GlobalFunc *global, LWInterface *local,void *serverData );
XCALL_( int ) Master( long version, GlobalFunc *global, LWMasterHandler *local,void *serverData );

XCALL_( int ) CutShadow_Handler( long version, GlobalFunc *global, LWPixelFilterHandler *local,void *serverData );
*/
//XCALL_( int ) ImageFilter_Activate( long version, GlobalFunc *global, LWImageFilterHandler *local,void *serverData );
//XCALL_( int ) ImageFilter_PortFog( long version, GlobalFunc *global, LWImageFilterHandler *local,void *serverData );
/* the server records */

//XCALL_( int )DepthSaver_Interface( long version, GlobalFunc *global, LWInterface *local,void *serverData );
//XCALL_( int )DepthSaver_Handler( long version, GlobalFunc *global, LWPixelFilterHandler *local,void *serverData );

//XCALL_( int )Atmosphere( long version, GlobalFunc *global, LWVolumetricHandler *local,void *serverData );
//XCALL_( int )Atmosphere_UI( long version, GlobalFunc *global, LWInterface *local, void *serverData );

//XCALL_( int )Sprite( long version, GlobalFunc *global, LWVolumetricHandler *local,void *serverData );
//XCALL_( int )Sprite_UI( long version, GlobalFunc *global, LWInterface *local, void *serverData );

///XCALL_( int ) Interface_Barn( long version, GlobalFunc *global, LWInterface *local,void *serverData );
//XCALL_( int ) Handler_Barn( long version, GlobalFunc *global, LWCustomObjHandler *local,void *serverData);

XCALL_( int )animaLoad( long version, GlobalFunc *global, LWObjectImport *local,void *serverData );
XCALL_( int )animaDisp( long version, GlobalFunc *global, LWDisplacementHandler *local, void *serverData);
XCALL_( int )animaMaster(long version,GlobalFunc  *global,LWMasterHandler *local,void *serverData);

ServerRecord ServerDesc[] = {
	
  // { LWMASTER_HCLASS,         MAST_PNAME,  Master      },
	   
  // { LWSHADER_HCLASS, "Stack_LimitShadow", LimitShadow_Handler },
  // { LWPIXELFILTER_HCLASS, "Stack_CutShadow", CutShadow_Handler },

  // { LWSHADER_HCLASS, "Demo_Blotch", Blotch_Handler },
  // { LWSHADER_ICLASS, "Demo_Blotch", Blotch_Interface },

   // { LWIMAGEFILTER_HCLASS, "Negative", ImageFilter_Activate },
 
  // { LWPIXELFILTER_HCLASS, "Depth Saver", DepthSaver_Handler },
   //{ LWPIXELFILTER_ICLASS, "Depth Saver", DepthSaver_Interface },
//	{ LWIMAGEFILTER_HCLASS, "PostFog", ImageFilter_PortFog },

//   { LWVOLUMETRIC_HCLASS, "Demo_GroundFog", Atmosphere },
 //  { LWVOLUMETRIC_ICLASS, "Demo_GroundFog", Atmosphere_UI },

   //{ LWVOLUMETRIC_HCLASS, "Stack_Sprite", Sprite },
   //{ LWVOLUMETRIC_ICLASS, "Stack_Sprite", Sprite_UI },

   { LWOBJECTIMPORT_CLASS, "AnimaLoad", animaLoad },

   { LWDISPLACEMENT_HCLASS, "Anima", animaDisp },

//   { LWMASTER_HCLASS, "EMotionMaster", emoMaster },
   //{ LWDISPLACEMENT_ICLASS, "LW_Inertia", Inertia_UI },

   //{ LWCUSTOMOBJ_HCLASS, "BasicBarn", Handler_Barn },
   //{ LWCUSTOMOBJ_ICLASS, "BasicBarn", Interface_Barn },

   { NULL }
};
