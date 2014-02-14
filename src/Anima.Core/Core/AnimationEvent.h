#pragma once

#include "Anima_nsbegin.h"
class AnimationPart;
class AnimClip;
class Model;
class ModelInstance;
class FilmLayer;
class AnimationPartClipSpline;
class AnimationPartStairs;
class MovePath;
class ContextIO;
class Clip;
class CoreFunction;
class AnimClipModel;

enum AnimationEventType
{
	CLIP = 1,
	PATH = 2,
	MODEL_OFFSET = 4,
	MODEL_SPEED = 8,

	CLIP_PATH = CLIP + PATH,
	MODEL = MODEL_OFFSET + MODEL_SPEED
};

class AnimationEvent
{
public:
	AnimationEventType type;
	ModelInstance *src_model;
	AnimationPart *src_part;
	//int actualRelativePartOffset;

	AnimationEvent();
	AnimationEvent(AnimationPart *src_part,bool clipChanged,bool pathChanged);
	AnimationEvent(AnimationPart *src_part,ModelInstance *model,bool clipChanged,bool pathChanged);

	int GetOffset(AnimationPart *part);

	// point
	bool HavePrevPointChanged(AnimationPart *part);
	bool HaveNextPointChanged(AnimationPart *part);
	bool GetPrevPoint(AnimationPart *part,Ogre::Vector3 &p);
	bool GetPrevNormal(AnimationPart *part,Ogre::Vector3 &n);
	bool GetNextPoint(AnimationPart *part,Ogre::Vector3 &p);
	bool GetNextNormal(AnimationPart *part,Ogre::Vector3 &n);
	// clip
	bool IsClipChanged(AnimationPart *part);
	//model
	bool IsModelChanged(AnimationPart *part);
};

#include "Anima_nsend.h"
