#pragma once

#include "Core/ModelLibrary.h"

#include "Anima_nsbegin.h"

class Model;
class MeshEditable;
class Clip;
class GfxClip;
class MovePath;
class AnimClipModel;
//class AnimClipModelGfx;
class ModelInstance;
class GfxClipAnim;
class Body;
class ClipBlend;
class GfxBaseModel;
class GfxModel;
class GfxModelMaterialEdit;

class ANIMA_CORE_API GfxModelPoseBone{
public:
	Ogre::Bone *bone;
	int rx; // degrees
	int ry;
	int rz;
	float dx;
	float dy;
	float dz;
	float sx;
	float sy;
	float sz;
	GfxModelPoseBone();
	void Clear();
	bool IsNull();
};

class ANIMA_CORE_API GfxModelPoseBone_Studio{
public:
	QString name;
	double value;
	GfxModelPoseBone_Studio();
	void Clear();
};

class ANIMA_CORE_API GfxModelPose_Studio{
public:
	QMap<QString,QString > boneAssignMap;
	void Load(QDomElement e);
	void Save(QDomDocument &doc,QDomElement &parentEle);
};

class ANIMA_CORE_API GfxModelPoseStorage
{
public:
	QDomDocument storeInfo;
};

class ANIMA_CORE_API GfxModelPose{

private:
	bool init;
	bool changed;
	GfxModel *model;
	int preRotXZ;
	Ogre::Animation *animation;
	QList<GfxModelPoseBone *> boneList;
	QList<GfxModelPoseBone_Studio *> boneStudioList;
	GfxModelPose_Studio *poseStudio;
	GfxModelPoseStorage *storage;

private:
	void EnsureInit();

public:

	GfxModelPose(GfxModel *model);
	~GfxModelPose();
	
	void Compute(Ogre::Entity *entity );

	QList<GfxModelPoseBone_Studio *> &GetBoneStudio(){return boneStudioList;}
	GfxModelPose_Studio *GetPoseStudio(){return poseStudio;}

	GfxModelPoseBone *GetBone(Ogre::Bone *bone);
	void Clear();
	void Invalidate();
	int GetPreRotXZ(){return preRotXZ;}
	void SetPreRotXZ(int preRotXZ);

	// IO
	void Load(QDomElement e);
	void Save(QDomDocument &doc,QDomElement &parentEle);
};

enum ANIMA_CORE_API GfxModelMaterialEdit_TextureChannel{
	GfxModelMaterialEdit_TextureChannel_ColorMap=1,
	GfxModelMaterialEdit_TextureChannel_SpecularMap=2,
	GfxModelMaterialEdit_TextureChannel_NormalMap=3,
	GfxModelMaterialEdit_TextureChannel_MaskMap=4
};

class ANIMA_CORE_API GfxModelTexture{
	friend class GfxModelMaterial;
private:
	QString import_path;
	QString path;
	bool absoluteMode;

public:
	GfxModelTexture();
	GfxModelMaterialEdit_TextureChannel ch;

	bool hasPath();
	void Save(QDomDocument &doc,QDomElement &parentEle);
	void SaveOgre(QTextStream &out);

	bool IsAbsoluteMode(){return absoluteMode; }
	void SetAbsoluteMode(GfxModel *model,bool );

	QString GetImportPath(){return import_path;}
	QString GetPathName();
	void SetPath(GfxModel *model,const QString &path);
	QString GetFullPath(GfxModel *model);
};

class ANIMA_CORE_API GfxModelMaterial{
private:
	GfxModelMaterialEdit *materialEdit;
	QString name;

	bool isMayaNormal;
	QString shader;
	double specularMultiply;
	double specularExponent;
	double fresnelExponent;
	GfxModelTexture colorMap;
	GfxModelTexture specularMap;
	GfxModelTexture normalMap;
	GfxModelTexture maskMap;

public:
	GfxModelMaterial(GfxModelMaterialEdit *materialEdit,const QString &name);

	QString GetName(){return name;} 

	QString GetShader(){return shader;}
	void SetShader(const QString &shader);

	bool IsMayaNormal(){return isMayaNormal;}
	void SetMayaNormal(bool );
	bool IsAbsoluteMode();
	void SetAbsoluteMode(GfxModel *model,bool absPath);

	double GetSpecularMultiply(){return specularMultiply;}
	double GetSpecularExponent(){return specularExponent;}
	double GetFresnelExponent(){return fresnelExponent;}

	//	specularMultiply (0.1-10)
	//	specular Exponent (20-200)
	//	fresnel Exponent (0.5 - 0.001)
	void SetIntensityGlossFresnel(double mult,double exp,double fresnel);

	//void SetMap(GfxModelMaterialEdit_TextureChannel ch,const QString &path,bool absoluteMode);
	GfxModelTexture *GetTextureMap(GfxModelMaterialEdit_TextureChannel ch);

	void Load(QDomElement e);
	void Save(QDomDocument &doc,QDomElement &parentEle);
	void SaveOgre(QTextStream &out);
};

class ANIMA_CORE_API GfxModelMaterialEdit{
private:
	GfxModel *model;
	QMap<QString,GfxModelMaterial *> matMap;

public:
	GfxModelMaterialEdit(GfxModel *model);
	~GfxModelMaterialEdit();

	void AddMaterial(GfxModelMaterial *mat);
	GfxModelMaterial *GetMaterial(const QString &);

	// IO
	void Load(QDomElement e);
	void Save(QDomDocument &doc,QDomElement &parentEle);
	void SaveOgre(QTextStream &out);
	void SaveOgre();
	void UpdateOgre();
};

class ANIMA_CORE_API GfxModel: public BObject
{
private:

	bool loaded;
	bool invalid;

	GfxBaseModel *gfxBaseModel;
	Ogre::Mesh* mesh;
	Ogre::SkeletonPtr skeletonPtr;
	Ogre::Skeleton* skeleton;
	Ogre::Skeleton::BoneHandleMap boneHandleMap;

	Model *model;
	QHash<Clip *,GfxClip *> clipMap;
	Body *infoBody;
	Body *taskBody;
	bool disposing;
	GfxModelPose *pose;
	GfxModelMaterialEdit *materialEdit;

	int vertexCount;
	int facesCount;
	GfxModelPoseStorage *matEditStorage;

private:

	void EnsureLoaded();

public:
	GfxModel(Model *model);
	~GfxModel(void);

	bool IsValid(){return !invalid;}

	QString GetName();
	Model *GetModel(){return model;}

	GfxBaseModel *GetBaseModel();
	Ogre::Mesh *GetMesh();
	Ogre::Skeleton *GetSkeleton();
	GfxModelPose *GetModelPose(){ return pose;}
	GfxModelMaterialEdit *GetMaterialEdit(){ return materialEdit;}
	
	bool Rename(const QString &newName);

	//Merge
	void EnsureValidAnimation(const Ogre::String &id);
	void FreeAnimation(const Ogre::String &id);

	GfxClip *GetClip(Clip *clip);
	GfxClip *GetClip(const QString &id);
	
	Body *GetInfoBody(){EnsureLoaded();return infoBody;}
	Body *GetTaskBody(){EnsureLoaded();return taskBody;}

	int GetVertexCount(){return vertexCount;}
	int GetFacesCount(){return facesCount;}

	void Unload();

	void AddResource();
	void ClearResource();

	// IO
	void Load(QDomElement e);
	void Save(QDomDocument &doc,QDomElement &parentEle);
};
#include "Anima_nsend.h"