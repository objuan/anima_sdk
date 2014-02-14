#pragma once
#include "BuildDispatcher.h"
#include "Anima_nsbegin.h"
class Animation;
class AnimationType;
class AnimationPart;
class AnimClipModel;
class ModelInstance;
class CoreFunction;
//class ModelInstanceIAPath;
class KeySpeedFunction;

/*
struct CoreFunction_Key
{
	double inSpace; // spazio dalla key alla precedente
	double globalSpace;
	double value;
	double time; // tempo dall'ultimo key
	double vm; // velocità media dalla key alla precedente
	CoreFunction_Key(double globalSpace,double value)
	{
		this->globalSpace=globalSpace;
		this->value=value;
		inSpace=-1;
		time=0;
		vm=0;
	}
};

class ANIMA_CORE_API CoreFunction_AnimEntry :public BObject
{
public:
 
	KeySpeedFunction *speedFun;
	AnimClipModel *animClip;
	AList<CoreFunction_Key> keyList; // gli estremi sono i bound key
//	bool isConstant;
	double clipTime;
	
	CoreFunction_AnimEntry(KeySpeedFunction *speedFun,AnimClipModel *animClip);

	void AddKey(double clipSpace,double value);
	void ClearKeys();
	void SetCostant(double val);
	void CopyFrom(CoreFunction_AnimEntry &fromEntry);
	void Build();
	double GetClipSpace();
	double GetSpeed(int startKey,double localTime);
	double GetSpeedBySpace(int startKey,double localSpace);
	double GetSpace(int startKey,double localTime);
	double GetLocalTime(int startKey,double localSpace);
	double GetKeyFreeSpace();
	double GetAnimClipInternalTime(double animClipTime,double &space,bool innerBaseTime );

	//double GetValue(double innerSpace,double &innerTime);
};

class ANIMA_CORE_API CoreFunction_Mark :public BObject
{
public:
	CoreFunction_AnimEntry *animEntry;
	double space; // 0 to 1, mai sui bordi
	double userVal; // multiplier 

	bool isBound;
	bool userEnabled;
	
	CoreFunction_Mark(CoreFunction_AnimEntry *animEntry,bool leftBound);

	CoreFunction_Mark(CoreFunction_AnimEntry *animEntry,double space,double val)
	{
		this->animEntry=animEntry;
		this->space=space;
		this->userVal=val;
		isBound=false;
		userEnabled=true;
	}
	void SetVal(double value);
	double GetRowValue();
	double GetValue(double &innerTime);
	double GetPercentSpace(){return space;}
	double GetClipSpace();

	bool IsClipDisabled();
	bool IsUserEnabled(){return userEnabled; }
	bool IsEnabled();
};

class ANIMA_CORE_API CoreFunction: public IBuildElement,public BObject {
public:
	enum CoreFunctionEditMode
	{
		CoreFunctionEditMode_Constant,
		CoreFunctionEditMode_User
	//	CoreFunctionEditMode_Auto
	};
protected:

	Animation *anim;
	ModelInstance *modelInstance;

public:
	CoreFunction(Animation *anim,ModelInstance *modelInstance);
	virtual ~CoreFunction();

	virtual bool IsEditable()=0;
	virtual CoreFunctionEditMode GetEditMode()=0;

	virtual double GetAnimClipTime(AnimClipModel *clip)=0;
	virtual double GetAnimClipInternalTime(AnimClipModel *clip,double animClipTime,double &space,bool innerBaseTime )=0;
	virtual void Clear()=0;

	virtual QString Serialize()=0;
	virtual void ResumeFromSerial(QString &)=0;

	virtual void InvalidateBuild(int type=0)=0;
	virtual void Dump()=0;
};

// stategia: per ogni parte, 0-1 sulla curva principale, es . 0-1 prima, 1-2 seconda ..

class ANIMA_CORE_API KeySpeedFunction: public CoreFunction {

private:

	AList<CoreFunction_AnimEntry> *entryList;
	AList<CoreFunction_Mark> markList;
	CoreFunctionEditMode editMode;
	bool cleared;

private:
	
	void _BuildFromMark();

public:
	KeySpeedFunction(Animation *anim,ModelInstance *modelInstance);
	virtual ~KeySpeedFunction();

//	void Init();
	virtual bool IsEditable(){return true;}

	CoreFunction_AnimEntry *GetEntry(int idx);
	int GetEntryCount(){return entryList->count();}
	int FindEntry(AnimClipModel *animClip);
	int GetMarkPartIndex(CoreFunction_Mark &mark);
	AList<CoreFunction_Mark> &GetMarkList(){ return markList;}
	CoreFunctionEditMode GetEditMode(){return editMode;}
	void SetEditMode(CoreFunctionEditMode mode);

	//void OnAnimClipsChanged();
	//void OnAnimBoundsChanged();
	void Build();
	void _BuildBounds(); // uso interno

	virtual double GetAnimClipTime(AnimClipModel *clip);
	double GetAnimClipSpacePerc(AnimClipModel *clip,double animClipTime);
	double GetAnimClipGlobalTime(AnimClipModel *clip,double internalFromTime ,double internalToTime);
	virtual double GetAnimClipInternalTime(AnimClipModel *clip,double animClipTime,double &space,bool innerBaseTime );
	double GetTime(double space);

	void DeleteMark(int idx,bool rebuild=true);
	void AddMark(AnimClipModel *clip,double spacePerc,double valPerc,bool rebuild);
	//void Update();
	void SetAllConstant(double valPerc);

	//void Copy(CoreFunction *fun);
	virtual void Dump();
	virtual void Clear();

	void Resize(AnimClipModel *clip,double timeOffset);
	void ClearBound(AnimClipModel *clip);
	void ClearClip(AnimClipModel *clip);

//	void Apply(ModelInstanceIAPath *path);

	QString Serialize();
	void ResumeFromSerial(QString &);

	virtual void InvalidateBuild(int type=0);
};


*/
#include "Anima_nsend.h"
