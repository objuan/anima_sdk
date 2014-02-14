#pragma once

#include "Core/ModelLibrary.h"

#include "Anima_nsbegin.h"

class BaseModel;
class MeshEditable;
class Clip;
class GfxBaseModelClip;
class MovePath;
class AnimClipModel;
//class AnimClipModelGfx;
class ModelInstance;
class IMesh;
class GfxTexture;
class GfxClipAnim;
class Body;
class ClipBlend;
class GfxOverlayClip;
class OverlayClip;

//class AnimPhysicController;

class ANIMA_CORE_API GfxBaseModel : public BObject
{
	friend class GfxContext;
//	friend class ImportFBX;

private:
	BaseModel *modelBase;
	QHash<Clip *,GfxBaseModelClip *> clipMap;
	QHash<OverlayClip *,GfxOverlayClip *> overlayClipMap;
	int lev;

public:

	int referenceCount;
	int hipIdx;
	int leftFootIdx;
	int rightFootIdx;
	int leftLegIdx;
	int rightLegIdx;

private:

	void LoadAll();

public:
	GfxBaseModel(BaseModel *model);
	~GfxBaseModel(void);

	QString GetName();
	BaseModel *GetBaseModel(){return modelBase;}

	//Ogre::Mesh *GetMesh(){return mesh;}
	Ogre::Skeleton *GetBaseSkeleton();

	void Lock();
	void Unlock();

	int GetHipIdx(){return hipIdx;}
	int GetLeftLegIdx(){return leftLegIdx;}
	int GetRightLegIdx(){return rightLegIdx;}

	// clips, se non c'e' prova a caricarla
	GfxBaseModelClip *GetClip(Clip *clip);
	GfxBaseModelClip *GetClip(const QString &id);

	GfxOverlayClip *GetOverlayClip(OverlayClip *clip);
	GfxOverlayClip *GetOverlayClip(const QString &id);


};
#include "Anima_nsend.h"