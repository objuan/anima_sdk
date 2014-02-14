#pragma once

#include "Context.h"
#include "Anima_nsbegin.h"

class Model;
class Clip;
class ClipLink;
class ModelClipFootInfo;
class GfxBaseModel;
class BaseModel;
class ClipBlend;

struct FixedPathLink
{
	double pre_clip_rot; 
	double direction_offset;

	FixedPathLink(): pre_clip_rot(0),direction_offset(0)//, splineStep1(0),splineStep2(0)
	{
	}
};

class ANIMA_CORE_API ClipBlendBoneAssign : public BObject 
{
public:
	QString clipID;
	double weight;
	int count;
	ClipBlendBoneAssign(const QString &clipID,double w) : clipID(clipID),weight(w),count(1)
	{
	}
};

class ANIMA_CORE_API ClipBlendBone : public BObject 
{
public:
	bool used;
	Ogre::Bone *bone;
	QList<ClipBlendBoneAssign *> assignList;

	ClipBlendBone(Ogre::Bone *bone) : bone(bone){}
	~ClipBlendBone();
	void Clear();
	bool HaveClip(const QString &clipID);
	void Add(const QString &clipID,double w);
	void Normalize();
	QString Dump();
};

struct ClipBlendAssignInfo
{
	QString clipID;
	int type;
	ClipBlendAssignInfo(const QString &clipID,int type):clipID(clipID),type(type){}
};

class ANIMA_CORE_API ClipBlendAssign : public BObject 
{
public:
	BaseModel *baseModel;
	Ogre::SkeletonInstance* skeleton;
	Ogre::Bone *rootBone;
	QMap<Ogre::Bone *,ClipBlendBone *> boneAssign;

	// debug info
	QStringList clipList;
	QList<ClipBlendAssignInfo> activeClipList;
	double prevTime;
	double nextTime;
	ClipBlend *blend_prev;
	ClipBlend *blend_next;


public:
	ClipBlendAssign(Ogre::SkeletonInstance* skeleton,BaseModel *baseModel);
	~ClipBlendAssign();
	void Clear();
	void Add(Ogre::Bone *bone,QList<Ogre::Bone *> &exclude,const QString &clipID,double w,bool recursion);
	void Add(const QString boneName,QStringList &boneExclude,bool defPhase,const QString &clipID,double w);
	void AddActiveClip(const QString &clipID,int type);
	void Normalize();
	void GetClipIDList(QStringList &clipList);
	QString Dump();
};

// -----------

class  ClipBlendFunctionPart : public BObject 
{
public:
	virtual double Evaluate(double time)=0;
};

class  ClipBlendFunctionPart_Line : public ClipBlendFunctionPart
{
public:
	double start;
	double end;

	ClipBlendFunctionPart_Line(){}
	ClipBlendFunctionPart_Line(double start,double end):start(start),end(end) {}

	virtual double Evaluate(double time);
};

class ClipBlendFunctionPart_Composite : public ClipBlendFunctionPart
{
public:
	QList<double> partTime;
	QList<ClipBlendFunctionPart*> partList;

	~ClipBlendFunctionPart_Composite();

	virtual double Evaluate(double time);
};

class  ClipBlendFunctionPart_Complementary : public ClipBlendFunctionPart
{
private:
	ClipBlendFunctionPart *part;
public:
	ClipBlendFunctionPart_Complementary(ClipBlendFunctionPart *part):part(part){}
	virtual double Evaluate(double time);
};

class  ClipBlendFunction : public BObject 
{
public:
	ClipBlendFunctionPart *fromPart;
	ClipBlendFunctionPart *toPart;

private:
	ClipBlendFunctionPart *ParsePart(QDomElement &ele,double &time);
	ClipBlendFunctionPart *Parse(QDomElement &parentEle);

public:
	ClipBlendFunction():fromPart(NULL),toPart(NULL){}
	~ClipBlendFunction();
	
	void Load(QDomElement &parentEle);

	double Evaluate(bool fromPhase,double time);
};

class ANIMA_CORE_API ClipBlendChannel: public BObject 
{
private:
	bool isCopy;
public:
	ClipBlend *clipBlend;
	QString ID;
	ClipBlendChannel *extend;
	QList<Ogre::Bone *> boneList;

	QString bone;
	QStringList boneExclude;
	double from_time;
	double to_time;
	bool enabled;
	ClipBlendFunction *fromClipfun;
	ClipBlendFunction *toClipfun;

private:
	void Parse(ClipBlendFunction **fun,QDomElement &ele,const QString &id);
public:
	ClipBlendChannel(ClipBlend *clipBlend): isCopy(false),clipBlend(clipBlend),fromClipfun(NULL),toClipfun(NULL),extend(NULL),bone(""){}
	~ClipBlendChannel();
	void Load(QDomElement &parentEle);
	bool GetActive(bool fromPhase,double time,double &val_from,double &val_to);
	bool Evaluate(bool fromPhase,bool defPhase,double time,const QString prevClipID,const QString nextClipID,ClipBlendAssign &assign);
};

class ANIMA_CORE_API ClipBlend : public BObject 
{
public:
	BaseModel *baseModel;
	QString fromClipID;
	QString toClipID;
	QList<ClipBlendChannel *> channelList;
public:

	ClipBlend(BaseModel *baseModel):baseModel(baseModel){}
	~ClipBlend();
	void Load(QDomElement &parentEle);
	void GetBones(QStringList &boneList);
	ClipBlendChannel *GetChannel(const QString boneName);
	static bool Evaluate(Ogre::SkeletonInstance* skeleton,ClipBlendAssign &assign,const QString prevClipID,const QString clipID,const QString nextClipID,ClipBlend *c_prev,ClipBlend *c_next,double prec_time,double next_time);

	void BeginClipBlendAssign(ClipBlendAssign &assign);
};


#include "Anima_nsend.h"
