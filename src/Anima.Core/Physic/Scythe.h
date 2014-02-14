#pragma once
#include "Anima_nsbegin.h"

class EdPrimitive;
class Actor;
class ScytheBone
{
public:
	ScytheBone() { dependantActor = 0; bone = 0; entity = 0; node = 0; prim = 0; owner = 0; }
	Ogre::String	    name;
	Ogre::Bone*			bone;
	Ogre::Vector3		pos;	//relative to model pos
	Ogre::Matrix3		rot;	//bones real rotation

	bool					isActor;		//true if connected to a physics actor
	bool					isRoot;		//true if this is the root of the skeleton



	//EDITOR ONLY
	Ogre::Quaternion		displayRot;		//bones rotation pointing to its parent, used only for display
	Ogre::Entity*			entity;
	Ogre::SceneNode*		node;
	EdPrimitive*			prim;		//not saved, but used for selecting
	EdPrimitive*			owner;	//model with the skeleton that we are part of
	void					scale(Ogre::Vector3 s);	//update correct scale

	//SIMULATION ONLY
	QList<ScytheBone*>	childBones;
	void				render(int skeletalID,  Ogre::Vector3 modelpos, Ogre::Quaternion modelrot, const Ogre::Quaternion parentsInitialWorldOrientation = Ogre::Quaternion::IDENTITY);
	void				renderDependants();
	Actor*			dependantActor;		//an actor with no primitives but has model(s) or light(s) that want to attach to the bone
};

class ScytheSkeleton
{
public:
 	ScytheSkeleton(){ pos = Ogre::Vector3(0,0,0); rot = Ogre::Quaternion::IDENTITY; }

	QList<ScytheBone*>		bonelist;
	Ogre::SkeletonInstance*		skel;
	ScytheBone*				getBone(const Ogre::String &name);
	bool					setup(Ogre::Entity* ent, EdPrimitive* ownerPrimitive);
	void					updateModelPos(Ogre::Vector3 pos, Ogre::Quaternion rot);	//tell the skeleton where the model is

	Ogre::Vector3			pos;
	Ogre::Quaternion			rot;
};


#include "Anima_nsend.h"