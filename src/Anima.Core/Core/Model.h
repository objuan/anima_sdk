#pragma once

#include "Item.h"
#include "Export/ParamsExporter.h"
#include "Clip.h"

#include "Anima_nsbegin.h"
class BaseItemLibrary;
class ModelClip;
class Action;
class MeshExtStatic;
class BaseModel;
class ModelExporter;
class ModelExporterParams;
class GfxModel;
class ModelEdit;

class ModelBound: public BObject
{
protected:
	Ogre::AxisAlignedBox bbox;

public:
	Ogre::AxisAlignedBox &GetBBox(){return bbox;}
	virtual double GetBBoxRadius()=0;
	virtual Ogre::Vector2 GetForce (double x,double y,ModelBound *bound,double bx,double by)=0;
};

class ModelBoundCircle : public ModelBound
{
public:
	double ray;

	ModelBoundCircle(double ray);
	virtual double GetBBoxRadius(){return ray;}
	Ogre::Vector2 GetForce (double x,double y,ModelBound *bound,double bx,double by);
};

enum ModelSkeletonMode
{
	ModelSkeletonMode_LINK,
	ModelSkeletonMode_MERGE
};

class ANIMA_CORE_API ModelURI  
{
private:
	QString uri;
	bool isTemporary;
public:
	ModelURI(){}
	ModelURI(const QString &uri);
	ModelURI(const QString &renderer,const QString &scene,const QString &meshURI,bool isTemporary=false);
	
	QString GetRenderer();
	QString GetScene();
	QString GetMeshURI();
	void SetTemporary(bool isTemporary){isTemporary=isTemporary;}
	bool IsTemporary(){return isTemporary;}

	QString ToString(){return uri; }
};

class ANIMA_CORE_API Model  : public Item
{
friend class GfxModel;

private:
	QString name;
	QString meshName;
	ModelURI uri;
	ItemCategoryCollection category;
	QColor color;
	BaseModel *base_model;
	QString desc;
	AnimatedImage image;
	QString iconName;
	float modelBaseSpeed;
	float modelSpeed;
	float basePhase;
	bool isVirtual;
	ModelBound *bound;
	ModelExporterParams *exportParams;
	ModelSkeletonMode skeletonMode;
	ModelEdit *modelEdit;

	Ogre::Vector3 legsRot;
	QList<Action *> actionList;
	QList<ModelClip *> clipList;
	QHash<QString,ModelClip *> *clipMap;

	GfxModel *gfxModel;

public:

	// graphics
	bool haveModel;
	int NodeForClip;
	MeshExtStatic *extInfo;

	// library
	bool Checked;
	QString LibraryName;
	QString Version;
	QDateTime VersionTime;
	QDateTime InsertTime;
	int ModelID;

private:	

	Ogre::Vector3 CopyTranslate(const Ogre::Vector3 &src);
	void CopyAnim(Ogre::Animation *fromAnim,double startTime,double endTime,Ogre::Skeleton* toSkeleton, Ogre::Animation *toAnim,bool isLoop);

public:
	Model();
	virtual ~Model(void);

	bool IsValid();
	GfxModel *GetGfxModel(){return gfxModel;}
	BaseModel *GetBaseModel(){return base_model;}
	ModelBound *GetBound(){return bound;}
	ModelExporterParams *GetExportParams(){return exportParams;}
	ModelExporter *GetExporter(ModelExporterType type,int version );
	void InitExportParams();

	ModelSkeletonMode GetSkeletonMode(){return skeletonMode; }
	void SetSkeletonMode(ModelSkeletonMode mode);
	ModelEdit *GetEdit(){return modelEdit;}

	ItemType GetItemType(){return ItemType_Model;}

	QString &GetName(){return name;}
	QString &GetMeshName(){return meshName;}
	ModelURI &GetModelURI(){return uri;}
	QString &GetDesc(){return desc;}
	QColor &GetColor(){return color;}
	ItemCategoryCollection &GetCategory(){return category;}
	AnimatedImage &GetImage(){return image;}
	Ogre::Vector3 &GetLegsRot(){return legsRot;}
	float GetModelSpeed(){return modelSpeed;}
	float GetModelBaseSpeed(){return modelBaseSpeed;}
	float GetBasePhase(){return basePhase;}
	float GetMinSpeed();
	float GetMaxSpeed();
	bool IsVirtual(){return isVirtual;}

	ModelClip *GetClip(Clip *clip);
	void GetClipCategoryList(QList<QString> &catList);
	void FindClips(const QString &categoryName,QList<Action *> &actions);

	// UI
	void BeginEdit();
	void SetMeshName(const QString &meshName);
	void SetModelSpeed(float speed);
	void SetPhase(float phase);
	void SetDesc(const QString &desc);
	void SetBoundRadius(float radius);
	void SetName(const QString &name);
	void SetColor(const QColor &color);
	void SetBaseModel(BaseModel *model);
	void SetModelURI(const ModelURI &uri);
	void EndEdit(bool unload=FALSE); // fa il refresh

	// IO
	void Init();
	void Load();
	void Load(QDomElement &parentEle);
	//void Save(QDomDocument &domDocument);
	void Save();
	void Save(QTextStream &out,QString &originalModelContent,const QString &clipID);
};

class ModelIO{
private:
	int level;
	QTextStream &out;
	int haveChilds[20];
private:
	void FillTabs();
public:
	ModelIO(QTextStream &out);

	void OpenElement(const QString &name);
	void CloseElement(const QString &name);
	void AddAttribute(const QString &name,const QString value);

};
#include "Anima_nsend.h"
