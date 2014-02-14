#pragma once

#include "ode/ode.h"

class GfxModel;
class AnimClipGfx;
class Traker;

class PBody;

class PBodyLeg
{
	friend class PBody;
private:
	dBodyID body[4];
	dMass mass[4];
	PBody *pBody;
	dJointID joint[4];
public:

	PBodyLeg(PBody *pBody,Traker *traker);

	void Copy(KTime time,Traker *traker);
	void Draw(KTime time);
};

class PBody {
friend class PBodyLeg;

private:

	dWorldID worldId;
	dJointGroupID contactgroup;

	//VectorR3 GetFeetOffset(KTime time);

	GfxModel *gfxModel;
	AnimClipGfx *animClipGfx;

	dMass hipMass;
	dBodyID hip;
	PBodyLeg *leftLeg;

public:
	PBody(AnimClipGfx *animClipGfx);
	~PBody();

	//void BuildRot(AnimClipGfx *animClipGfx,Traker *traker,KTime &time);
	//void ComputeLegMove(AnimClipGfx *animClipGfx,KTime time,bool offsetEnable);
	void ProjectToModel(Traker *traker,char *animName);

	void Draw(KTime time);
};

