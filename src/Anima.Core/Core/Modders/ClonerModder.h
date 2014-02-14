#pragma once

#include "../Modder.h"

#include "Anima_nsbegin.h"

class Animation;
class ClonerModder_Area_Mask;
class ClonerModder_Rotation;

struct ClonerModder_AreaPos 
{
public:
	ModelInstance *mi;
	double x;
	double y;
	double z; // solo maschere
	bool absolutePosition;
	bool fixed;
	double dx;
	double dy;
	bool invert;
	ClonerModder_AreaPos(ModelInstance *mi,double x,double y,double z,bool absolutePosition,bool fixed,bool invert) : mi(mi),x(x),y(y),z(z),absolutePosition(absolutePosition),fixed(fixed),invert(invert){}
};

enum ClonerModder_Area_FullRectangleAlignType
{
	ClonerModder_Area_FullRectangleAlignType_CENTER = 0,
	ClonerModder_Area_FullRectangleAlignType_LEFT = 1,
	ClonerModder_Area_FullRectangleAlignType_RIGHT= 2
};

class ClonerModder_Area : public BObject
{
protected:
	QList<ClonerModder_AreaPos> posList;
private:
	double ComputeDir(int idx);
	Ogre::Vector2 GetForce (ClonerModder_AreaPos &pos1,ClonerModder_AreaPos &pos2);
protected:
	void Begin();
	void Add(Animation *anim,ModelInstance *model,double x,double y,double z,bool absolutePosition,bool invert,ClonerModder_Area_FullRectangleAlignType align);
	void End(Animation *anim,bool scale=true);

	void SetModel(ModelInstance *model,double dir_offset,double dir_offset_90);
public:

	virtual double GetArea()=0;
	virtual void SetArea(double area)=0;

	virtual void Process(Animation *anim,QList<ModelInstance *> models ,int seed,bool useInvert){};
};

class ClonerModder_Area_Circle : public ClonerModder_Area
{
public:

	double ray;

	ClonerModder_Area_Circle(){}
	virtual double GetArea(){return ray * ray * 3.1415;}
	virtual void SetArea(double area);
	void SetRay(double *ray,double minArea);

	virtual void Process(Animation *anim,QList<ModelInstance *> models ,int seed,bool useInvert);
};

class ClonerModder_Area_Rectangle : public ClonerModder_Area
{
public:

	double width;
	double height;

	virtual double GetArea(){return width * height;}
	virtual void SetArea(double area);
	void SetBound(double *w,double *h,double minArea);

	ClonerModder_Area_Rectangle(){}

	virtual void Process(Animation *anim,QList<ModelInstance *> models ,int seed,bool useInvert);
};


class ClonerModder_Area_FullRectangleAlign
{
public:
	ClonerModder_Area_FullRectangleAlignType type;
	double offset;

	ClonerModder_Area_FullRectangleAlign():offset(1){}
};

class ClonerModder_Area_FullRectangle : public ClonerModder_Area
{
public:

	Animation *anim;
	//double offset;
	double lenghtMeter;
	double lenghtPerc;
	bool lenghtModePerc;
	ClonerModder_Area_FullRectangleAlign leftAlign; 
	ClonerModder_Area_FullRectangleAlign centerAlign; 
	ClonerModder_Area_FullRectangleAlign rightAlign; 

	ClonerModder_Area_FullRectangleAlign *currentAlign;

	double modelTime;
	double modelSpeed;

	virtual double GetArea();
	virtual void SetArea(double area);
	void OnSizeChanged();
	void SetBound(double *len,double *offset,double minArea);

	ClonerModder_Area_FullRectangle(Animation *anim):anim(anim),currentAlign(&centerAlign),lenghtMeter(1),lenghtPerc(100),lenghtModePerc(true){}

	virtual void Process(Animation *anim,QList<ModelInstance *> models ,int seed,bool useInvert);
};

class ANIMA_CORE_API ClonerModder : public Modder , public ISceneObjectMovable
{
public:
	enum ClonerModder_Area_Type
	{
		ClonerModder_Area_Type_Undefined = -1,
		ClonerModder_Area_Type_FullRectangle = 0,
		ClonerModder_Area_Type_Circle = 1,
		ClonerModder_Area_Type_Rectangle = 2,
		ClonerModder_Area_Type_Mask = 3
	};
private:
	int modelCount;
	bool lockMode;
	double userArea;
	bool useInvert;
	bool firstTime;

	ClonerModder_Area_Rectangle area_rect;
	ClonerModder_Area_Circle area_circle;
	ClonerModder_Area_FullRectangle *area_fullrect;
	ClonerModder_Area_Mask *area_mask;

	ClonerModder_Area *area;
	double model_random_seed;
	double area_random_seed;
	//double pos_rotation_random_seed; // <1 disabled
	double phase_random_seed; // <1 disabled
	double speed_random_seed; // <1 disabled
	double overlay_random_seed; // <1 disabled

	Animation *anim;
	ClonerModder_Rotation *rotationInfo;

private:
	void Check();
	double GetModelMinArea();
	double GetAreaMinModel();

public:
	ClonerModder(Animation *anim);
	virtual ~ClonerModder(void);

	ClonerModder_Rotation *GetRotationInfo(){return rotationInfo;}

	virtual ModderChildType GetModderType(){return ClonerModderType;}
	virtual QColor GetMaterialColor();
	QImage Image(){return ResourceManager::Singleton.item_cloner;}
    virtual QString ClassDesc(){return "Cloner";}

	virtual void OnInit();
	virtual void OnDelete();

	ClonerModder_Area_Type GetAreaType();
	ClonerModder_Area *GetArea(){return area;}
	void SetArea(ClonerModder_Area_Type type);

	bool GetLockMode(){ return lockMode;}
	void SetLockMode(bool lockMode);

	double GetUserArea(){ return userArea;}
	void SetUserArea(double *userArea); // controlli di validità
	void SetAreaValues(double *v1,double *v2);  // a seconda del tipo,controlli di validità

	double GetModelCount(){ return modelCount;}
	void SetModelCount(int *modelCount); // controlli di validità

	double GetModelRandomSeed(){ return model_random_seed;}
	void SetModelRandomSeed(double model_random_seed){this->model_random_seed=model_random_seed;}

//	bool GetPosRotationRandomSeedEnabled();
//	double GetPosRotationRandomSeed(){ return pos_rotation_random_seed;}
//	void SetPosRotationRandomSeed(double pos_rotation_random_seed){this->pos_rotation_random_seed=pos_rotation_random_seed;}

	double GetAreaRandomSeed(){ return area_random_seed;}
	void SetAreaRandomSeed(double area_random_seed){this->area_random_seed=area_random_seed;}

	double GetPhaseRandomSeed(){ return phase_random_seed;}
	void SetPhaseRandomSeed(double phase_random_seed){this->phase_random_seed=phase_random_seed;}

	double GetSpeedRandomSeed(){ return speed_random_seed;}
	void SetSpeedRandomSeed(double speed_random_seed){this->speed_random_seed=speed_random_seed;}

	double GetOverlayRandomSeed(){ return overlay_random_seed;}
	void SetOverlayRandomSeed(double overlay_random_seed){this->overlay_random_seed=overlay_random_seed;}

	bool GetUseInvert(){return useInvert;}
	void SetUseInvert(bool _useInvert){useInvert=_useInvert;}

	ClonerModder_Area_Rectangle &_getAreaRect(){return area_rect;}
	ClonerModder_Area_Circle &_getAreaCircle(){return area_circle;}
	ClonerModder_Area_FullRectangle &_getAreaFullRectangle(){return *area_fullrect;}
	ClonerModder_Area_Mask &_getAreaMask(){return *area_mask;}

	void OnMeshUnitChanged();

	virtual void Build();
	virtual void InvalidateBuild(int type=0);

	virtual void Load(QDomElement e);
	virtual void Save(QDomElement e);

	// MOVE API

	virtual Ogre::Vector3 &GetPos();
	virtual void GetRot(float &_rot_xz);
	virtual void SetPos(const Ogre::Vector3 &pos);
	virtual void SetRot(float rot_xz);
	virtual void GetLocalTrx(Ogre::Matrix4 &mat);

};
#include "Anima_nsend.h"

