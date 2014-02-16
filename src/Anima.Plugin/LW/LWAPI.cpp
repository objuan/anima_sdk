#include "stdafx.h"
#include "LWApi.h"

#include "LWAnima.h"

AnimaStore *animaStore=NULL;

int __stdcall
LW_API_Load( GlobalFunc *global, LWObjectImport *local,void *serverData,char *programPath )
{
	QString a;
	Ogre::Vector3 v;
	Ogre::Vector3 v1;

	if (animaStore==NULL) animaStore = new AnimaStore(global,programPath); // new
	
	return animaStore->Load(local,serverData);
}

int __stdcall
LW_API_DisplacementInit( GlobalFunc *global,  LWDisplacementHandler *local, void *serverData,char *programPath )
{
	if (animaStore==NULL) animaStore = new AnimaStore(global,programPath); // new

	return animaStore->DisplacementInit(local,serverData);
}

// ========

void __stdcall
LW_API_Clear()
{
	/*if (animaStore == NULL) return;
	if (animaStore!=NULL) delete animaStore;
	animaStore = NULL;*/
}
void __stdcall
LW_API_NewTime(  AnimaData *dat, LWFrame fr, LWTime t,double fps )
{
	dat->frame = fr;
	dat->time = t;
	dat->vmId = NULL;  // reset now, re-aquire on first vertex of this pass

	animaStore->NewTime(dat,fr,t, fps);
}
void __stdcall
LW_API_DisplaceLWPoint( AnimaData *dat, LWDisplacementAccess *da )
{
	LWDVector pos, v, xcol, ycol, zcol, siz;
	double t, dist;
	LWMeshInfoID mesh;

	animaStore->DisplaceLWPoint(dat,da);
}

