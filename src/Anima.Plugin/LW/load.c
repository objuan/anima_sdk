/*
======================================================================
load.c

Load an object in VideoScape ASCII .geo format.

Ernie Wright  30 Mar 00
====================================================================== */

#include <windows.h>
#include <lwserver.h>
#include <lwobjimp.h>
#include <lwsurf.h>
#include <lwdisplce.h>
#include <lwhost.h>
#include <lwhandler.h>
#include <lwmeshes.h>
#include <lwdisplce.h>
#include <lwenvel.h>
#include <lwvparm.h>
#include <lwmath.h>
#include <lwmaster.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct {
   int   total;
   int   stepsize;
   int   nextstep;
} Progress;

typedef struct st_AnimaData {
   LWTime      time;          // current time
   LWFrame     frame;            // current frame
   LWItemID    item, pivot;         // self and optional pivot
   LWVParmID   lagParm;          // Enveloped parameter: lag, seconds of delay per unit of offset
   double      lagRate;          // cached lag value
   LWDVector   pivpos;           // cached center
   int         flags;            // use INF_ flag bit definitions
   int         vmapIdx, pvid;    // index into list
   void       *vmId;          // vmap id cached
   char        vmName[ 80 ];     // vmap name!
   char        desc[ 80 ];       // Your Message Here!
} AnimaData;

extern HMODULE hModule;

//int _emoDisp( GlobalFunc *global, LWObjectImport *local,void *serverData );

static LWObjectFuncs   *objf     = NULL;
static LWEnvelopeFuncs *envf     = NULL;
static LWChannelInfo   *chanf    = NULL;
static LWItemInfo      *iteminfo = NULL;
static LWVParmFuncs    *vparmf   = NULL;
static LWSceneInfo *sceneinfo  = NULL;

extern char out_programPath[1000];

// ========================================

int _LW_API_Load( GlobalFunc *global, LWObjectImport *local,void *serverData,char *programPath );

int _animaLoad( GlobalFunc *global, LWObjectImport *local,void *serverData )
{
	return _LW_API_Load(global,local,serverData,out_programPath);
}

int _LW_API_DisplacementInit( GlobalFunc *global, LWDisplacementHandler *local,void *serverData,char *programPath );

int _animaDisp( GlobalFunc *global, LWDisplacementHandler *local,void *serverData )
{
	return _LW_API_DisplacementInit(global,local,serverData,out_programPath);
}

int _LW_API_Clear( );

void _Clear()
{
	_LW_API_Clear();
}

void _LW_API_NewTime( AnimaData *dat, LWFrame fr, LWTime t,double fps  );

void _NewTime( AnimaData *dat, LWFrame fr, LWTime t,double fps   )
{
	_LW_API_NewTime(dat,fr,t,fps);
}

void _LW_API_DisplaceLWPoint(AnimaData *dat, LWDisplacementAccess *da  );

void _DisplaceLWPoint(  AnimaData *dat, LWDisplacementAccess *da )
{
	_LW_API_DisplaceLWPoint(dat,da);
}

// ==============================================================

XCALL_( static LWInstance ) Create( void *priv, LWItemID item, LWError *err )
{
   AnimaData *dat;
   LWChanGroupID cgroup;

   if ( dat = calloc( 1, sizeof( AnimaData ))) {
      dat->item = item;
     dat->vmName[0] = 0;
      if ( dat->lagParm = vparmf->create( LWVP_FLOAT, LWVPDT_NOTXTR )) {
         cgroup = iteminfo->chanGroup( dat->item );
         vparmf->setup( dat->lagParm, "LagRate", cgroup,
                                    NULL, NULL, NULL, NULL );
      }
   }
   return dat;
}



XCALL_( static void ) Destroy( AnimaData *dat )
{
   if( dat ) {
      if ( dat->lagParm )
         vparmf->destroy( dat->lagParm );
      free( dat );
   }
   _Clear();
}

XCALL_( static LWError ) Copy( AnimaData *to, AnimaData *from )
{
   LWItemID id;
   LWVParmID vpid;

   id = to->item;
   vpid = to->lagParm;
   *to = *from;
   to->item = id;
   to->lagParm = vpid;
   return vparmf->copy( to->lagParm, from->lagParm );
}
XCALL_( static const char * ) Describe( AnimaData *dat )
{
   sprintf( dat->desc, " Mover " );
   return dat->desc;
}

XCALL_( static LWError ) Init( AnimaData *dat, int mode )
{
   return NULL;
}
XCALL_( static void )Cleanup( AnimaData *dat )
{
   return;
}

XCALL_( static LWError )NewTime( AnimaData *dat, LWFrame fr, LWTime t )
{
	_NewTime(dat,fr,t,sceneinfo->framesPerSecond);
   //dat->frame = fr;
   //dat->time = t;
   //dat->vmId = NULL;  // reset now, re-aquire on first vertex of this pass

   ///* The following operations can be performed onse every time,
   //   rather than once per vertex. */

   ///* get the rotation origin for the pivot object */
   //if ( dat->pivot )
   //   iteminfo->param( dat->pivot,
   //     LWIP_POSITION //: LWIP_W_POSITION,
   //      ,dat->time, dat->pivpos );
   //else
   //   iteminfo->param( dat->item, LWIP_PIVOT, dat->time, dat->pivpos );

   ///* get the rate from the envelope */
   //vparmf->getVal( dat->lagParm, dat->time, NULL, &dat->lagRate );

   return NULL;
}

XCALL_( static int )Flags( AnimaData *dat )
{
   return LWDMF_WORLD;
}

XCALL_( static void )
Evaluate( AnimaData *dat, LWDisplacementAccess *da )
{
	_DisplaceLWPoint(dat,da);
	/*LWDVector pos, v, xcol, ycol, zcol, siz;
	double t, dist;
	LWMeshInfoID mesh;

	LWPntID point = da->point;
	int yy=0;

	da->source[1] = da->source[1]+5;*/
}


XCALL_( int )
animaDisp( long version, GlobalFunc *global, LWDisplacementHandler *local, void *serverData)
{
  int ret;

   if ( version != LWDISPLACEMENT_VERSION )
      return AFUNC_BADVERSION;

   objf     = global( LWOBJECTFUNCS_GLOBAL,   GFUSE_TRANSIENT );
   envf     = global( LWENVELOPEFUNCS_GLOBAL, GFUSE_TRANSIENT );
   chanf    = global( LWCHANNELINFO_GLOBAL,   GFUSE_TRANSIENT );
   iteminfo = global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT);
   vparmf   = global( LWVPARMFUNCS_GLOBAL,    GFUSE_TRANSIENT);
   sceneinfo = global( LWSCENEINFO_GLOBAL, GFUSE_TRANSIENT );

   if ( !objf || !envf || !chanf || !iteminfo || !vparmf )
      return AFUNC_BADGLOBAL;

   local->inst->create   = Create;
   local->inst->destroy  = Destroy;
   //local->inst->load     = Load;
   //local->inst->save     = Save;
   local->inst->copy     = Copy;
   local->inst->descln   = Describe;
/*
   local->item->useItems = UseItems;
   local->item->changeID = ChangeID;
*/
   local->rend->init     = Init;
   local->rend->cleanup  = Cleanup;
   local->rend->newTime  = NewTime;

   local->evaluate       = Evaluate;
   local->flags          = Flags;

    ret =_animaDisp(global,local,serverData,out_programPath);
	if (ret == 0)
	{
		// ok
		return AFUNC_BADGLOBAL;
	}

   return AFUNC_OK;
}

/*
======================================================================
animaLoad()

Activation function for the loader.
====================================================================== */


XCALL_( int )
animaLoad( long version, GlobalFunc *global, LWObjectImport *local,
   void *serverData )
{
   LWObjectFuncs *objfunc;
   static int isurf[ 261 ];
   Progress progress;
   struct stat s;
   char str[ 8 ], *err = NULL;
   int npts;
   FILE *fp = NULL;
   int i, j,c;
   char *fileName;
   int ret;

   //trace("xyz","emoLoad","");

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

     /* attempt to open the file */

	   fp = fopen( local->filename, "r" );
	   if ( !fp ) {
		  local->result = LWOBJIM_BADFILE;
		  return AFUNC_OK;
	   }

	   /* see whether this is a VideoScape ASCII object file;
		  if not, let someone else try to load it */

	  fread( str, 1, 7, fp );
	  if (strncmp( str, "<Anima>", 7 )) {
		  fclose( fp );
		  local->result = LWOBJIM_NOREC;
		  return AFUNC_OK;
	   }
	   else
		   fclose( fp );


    err = "Load error";

    local->result = LWOBJIM_FAILED;   /* assume this until we succeed */

   // -----------
	ret =_animaLoad(global,local,serverData,out_programPath);
	if (ret == 0)
	{
		// ok
		local->result = LWOBJIM_OK ;

		//global->
	}
	else if (ret == 1)
	{
		local->result = LWOBJIM_NOREC;
	}else if (ret == 2)
	{
		err ="Cannot load more than 2 .ani object,please delete before";
		local->result = LWOBJIM_FAILED;
	}
   /* we're done */

  
 /*   objfunc = global( LWOBJECTFUNCS_GLOBAL, GFUSE_TRANSIENT );
	c = objfunc->numObjects();
	for(i=0;i<c;i++)
	{
		fileName = objfunc->filename(i);

		local->result = LWOBJIM_OK;
	}*/

   MON_DONE( local->monitor );
   local->done( local->data );

Finish:
   if (( local->result != LWOBJIM_OK ) && err && ( local->failedLen > 0 ))
      strncpy( local->failedBuf, err, local->failedLen );
   return AFUNC_OK;
}

//==============================================================================


XCALL_(unsigned int) master_flags(LWInstance inst)
{
    return(0);
}

XCALL_(const char *) master_describe(LWInstance inst)
{
    return("Anima Master");
}

XCALL_(LWInstance) master_create(void *priv,void *context,LWError *err)
{
	 return NULL;
}

XCALL_(void) master_destroy(LWInstance inst)
{
	return NULL;
}

XCALL_(LWError) master_copy(LWInstance d1,LWInstance d2)
{
    XCALL_INIT;
    return(NULL);
}

XCALL_(LWError) master_load(LWInstance inst,const LWLoadState *lState)
{
    XCALL_INIT;
    return(NULL);
}

XCALL_(LWError) master_save(LWInstance inst,const LWSaveState *sState)
{
    XCALL_INIT;
    return(NULL);
}

XCALL_(double) master_process(LWInstance inst,const LWMasterAccess *ma)
{
}

XCALL_( int )
animaMaster(long version,GlobalFunc  *global,LWMasterHandler *local,void *serverData)
{
    if(version != LWMASTER_VERSION)
        return(AFUNC_BADVERSION);

    if(local->inst)
    {
        local->inst->create     = master_create;
        local->inst->destroy    = master_destroy;
        local->inst->load       = master_load;
        local->inst->save       = master_save;
        local->inst->copy       = master_copy;
        local->inst->descln     = master_describe;
    }

    if(local->item)
    {
        local->item->useItems   = NULL;
        local->item->changeID   = NULL;
    }

    local->event            = master_process;
    local->flags            = master_flags;

   // global = g;

//    message = (LWMessageFuncs *)(*global)("Info Messages",GFUSE_ACQUIRE);

    return(AFUNC_OK);
}
