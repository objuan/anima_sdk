#pragma once

#ifdef ANIMA_LW_EXPORTS
#define ANIMA_LW_API __declspec(dllexport)
#else
#define ANIMA_LW_API __declspec(dllimport)
#endif

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

int __stdcall
LW_API_Load( GlobalFunc *global, LWObjectImport *local,void *serverData,char *programPath );
int __stdcall
LW_API_DisplacementInit(  GlobalFunc *global, LWDisplacementHandler *local, void *serverData,char *programPath );

void __stdcall
LW_API_Clear();
void __stdcall
LW_API_NewTime( AnimaData *dat, LWFrame fr, LWTime t,double fps);
void __stdcall
LW_API_DisplaceLWPoint(AnimaData *dat, LWDisplacementAccess *da );


