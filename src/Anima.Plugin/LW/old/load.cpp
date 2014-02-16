//#include "Core/Context.h"
//#include "Core/Config.h"
//#include "Core/ResourceManager.h"
//#include "Gfx/GfxContext.h"

#include <lwserver.h>
#include <lwobjimp.h>
#include <lwsurf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct {
   int   total;
   int   stepsize;
   int   nextstep;
} Progress;

/*
======================================================================
emoLoad()

Activation function for the loader.
====================================================================== */

 //extern "C" void f(int i, char c, float x);

 extern "C" XCALL_( int )
_emoLoad( long version, GlobalFunc *global, LWObjectImport *local,void *serverData )
{
}

extern XCALL_( int )
emoLoad( long version, GlobalFunc *global, LWObjectImport *local,void *serverData )
{
   static int isurf[ 261 ];
   LWFVector pos = { 0.0f };
  // LWPntID *pntID = NULL;
   Progress progress;
   struct stat s;
   char str[ 6 ];
   char err[200];
   int npts;
   int i, j;


   /* check the activation version */

   if ( version != LWOBJECTIMPORT_VERSION ) return AFUNC_BADVERSION;

   /* get the file size */

   if ( stat( local->filename, &s )) {
      local->result = LWOBJIM_BADFILE;
      return AFUNC_OK;
   }

   progress.total = s.st_size;
   progress.stepsize = progress.total / 20;
   progress.nextstep = progress.stepsize;

   local->result = LWOBJIM_FAILED;   /* assume this until we succeed */

   strcpy(err,"Bad point count.");
 
   
	//try
	//{
	//	Config::Init();
	//	Context::Singleton.Init();

	//	ResourceManager::Singleton.Init();
	//	GfxContext::Singleton.Init();
	//}
	//catch(CoreException &e)
	//{
	//	err = "eMotion startup error";
	//	goto Finish;
	//}

   /* we're done */

   MON_DONE( local->monitor );
   local->done( local->data );

Finish:
 //  if ( fp ) fclose( fp );
   //if ( pntID ) free( pntID );
   if (( local->result != LWOBJIM_OK ) && err && ( local->failedLen > 0 ))
      strncpy( local->failedBuf, err, local->failedLen );
   return AFUNC_OK;
}
