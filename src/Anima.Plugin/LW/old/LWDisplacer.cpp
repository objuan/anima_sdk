#include "./EMotion.h"
#include "Core/Context.h"
#include "Core/Config.h"
#include "Core/ModelInstance.h"
#include "Core/Config.h"
#include "Gfx/GfxContext.h"
#include "Gfx/GfxModel.h"
#include "IO/ContextIO.h"

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

extern EMotionStore *eMotionStore;

typedef struct st_EMotionData {
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
} EMotionData;


extern "C"  void _NewTime( EMotionData *dat, LWFrame fr, LWTime t )
{
	dat->frame = fr;
	dat->time = t;
	dat->vmId = NULL;  // reset now, re-aquire on first vertex of this pass

	eMotionStore->NewTime(dat,fr,t);
}

extern "C" void _DisplaceLWPoint( EMotionData *dat, LWDisplacementAccess *da )
{
	LWDVector pos, v, xcol, ycol, zcol, siz;
	double t, dist;
	LWMeshInfoID mesh;

	eMotionStore->DisplaceLWPoint(dat,da);

	//LWPntID point = da->point;
	//if (basePoint==NULL)
	//{
	//	basePoint = point;
	//}
	//float idx = (float)(point-basePoint));

	//int yy=0;
	//da->source[1] = da->source[1]+5;
}