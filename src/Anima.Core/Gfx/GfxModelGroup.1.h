#pragma once

#include "Core/Film.h"

class ModelGroup;
class AnimClip;
class GfxClip;
class GfxModel;
class ModelGroupClip;
class IMovePath;

class EMOTION_CORE_API GfxModelGroup
{
private:

	ModelGroup *modelGroup;
	QList<GfxClip *> clipList;
//	ModelGroupClip *selectedClip;
	
private:

public:
	GfxModelGroup(ModelGroup *modelGroup);
	~GfxModelGroup(void);

	ModelGroup *GetModelGroup(){return modelGroup;}

	void OnLoad();

	GfxClip *FindClip(const QString &clipName);
	GfxClip *AddClip(AnimClip *clip);
	void DelClip(AnimClip *clip);

	void ClearClips();

};
