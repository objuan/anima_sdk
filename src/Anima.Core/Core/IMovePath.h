#pragma once

#include "../Anima.Core.h"

class AnimationPart;

class ANIMA_CORE_API IMovePath: public AObject
{
public:

	virtual AObject::Type GetType(){return AObject::Type::IMovePathType;}
	virtual QString ClassName(){return "IMovePath";}

	virtual bool GetPointAtDistance(double dist,KFbxVector4 &precPoint,KFbxVector4 &point)=0;
	virtual bool GetInfoAtDistance(double distance,KFbxVector4 &point,KFbxVector4 &dir)=0;

	virtual double Length()=0;
	virtual double Height()=0;
	virtual double GetClipCount(){return -1;} // nomero di clip che fanno il path, -1 se non e' importante
	virtual double GetClipW(){return -1;}
	virtual double GetClipH(){return -1;}
	//virtual bool GetFirst(KFbxVector4 &point,int offset=0)=0;
	//virtual bool GetLast(KFbxVector4 &point,int offset=0)=0;
	virtual bool GetFirstNormal(KFbxVector4 &normal)=0;
	virtual bool GetLastNormal(KFbxVector4 &normal)=0;
};
