#pragma once
#include "Anima_nsbegin.h"
class Body;
class Bone;

class ANIMA_CORE_API BodyPose: public BObject
{
private:
	Body *body;
	double time;

public:

	BodyPose(Body *body,int keyIdx);
	~BodyPose();
};
#include "Anima_nsend.h"