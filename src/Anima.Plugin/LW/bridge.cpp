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

extern "C" HMODULE hModule;

typedef int (CALLBACK* LW_API_Load_FUN)(GlobalFunc *,LWObjectImport *,void *,char *);

extern "C" int _LW_API_Load( GlobalFunc *global, LWObjectImport *local,void *serverData,char *programPath )
{
	LW_API_Load_FUN p_animaLoad = (LW_API_Load_FUN)GetProcAddress(hModule,"LW_API_Load");
	p_animaLoad(global,local,serverData,programPath);
	return 0;
}

typedef int (CALLBACK* LW_API_DisplacementInit_FUN)(GlobalFunc *,LWDisplacementHandler *,void *,char *);

extern "C" int _LW_API_DisplacementInit( GlobalFunc *global, LWDisplacementHandler *local,void *serverData,char *programPath )
{
	LW_API_DisplacementInit_FUN p_animaDisp = (LW_API_DisplacementInit_FUN)GetProcAddress(hModule,"LW_API_DisplacementInit");
	return p_animaDisp(global,local,serverData,programPath);
}

// ====================
extern "C"  void _LW_API_Clear()
{
	FARPROC m = GetProcAddress(hModule,"LW_API_Clear");
	m();
}

typedef int (CALLBACK* LW_API_NewTime_FUN)( AnimaData *, LWFrame , LWTime ,double);

extern "C"  void _LW_API_NewTime( AnimaData *dat, LWFrame fr, LWTime t,double fps )
{
	LW_API_NewTime_FUN m = (LW_API_NewTime_FUN)GetProcAddress(hModule,"LW_API_NewTime");
	m(dat,fr,t,fps);
}

typedef int (CALLBACK* LW_API_DisplaceLWPoint_FUN)(AnimaData *, LWDisplacementAccess *);

extern "C"  void _LW_API_DisplaceLWPoint( AnimaData *dat, LWDisplacementAccess *da )
{
	LW_API_DisplaceLWPoint_FUN m = (LW_API_DisplaceLWPoint_FUN)GetProcAddress(hModule,"LW_API_DisplaceLWPoint");
	m(dat,da);
}

