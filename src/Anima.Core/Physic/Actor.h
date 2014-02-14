#pragma once
#include "Scythe.h"

#include "Anima_nsbegin.h"

//class ScytheBone;
class PhysicsActor;
class PhysicsModel;

//a physics controlled model, these are sent to the client for rendering
//Actor just has 1 model
//Actor has no pos, pos is phys objects pos + axis offset
class Actor
{
public:
	Actor() { isSkeletal = false; bone = 0; }
	~Actor(){}

	long						index;
	//Physics plugin's object
	QList<std::string>			modelName;
	int						model;
	QList<Ogre::Vector3>				modelPos;		//positional offset
	QList<Ogre::Matrix3>			modelAxisMat;	//offset from actors axis
	QList<Ogre::Vector3>				modelScale;
	QList<Ogre::SceneNode*>		node;
	QList<Ogre::Entity*>		ent;

	//details for all the lights
	QList<Ogre::Vector3>				lightPos;		//positional offset
	QList<Ogre::Matrix3>			lightAxisMat;	//offset from actors axis
	QList<Ogre::Light*>		light;

	bool						isSkeletal;		//is a model with a skeleton, not a normal physics actor

	ScytheBone*				bone;			//if not 0, this actor contains a bone
	
	virtual void	render(const Ogre::Vector3 rigidBodyPos, const Ogre::Matrix3 rigidBodyRot);
	void	create(const PhysicsActor* act, const Ogre::Vector3 position, const Ogre::Matrix3 boneOffset = Ogre::Matrix3::IDENTITY);
	void	addModel(const PhysicsModel* model, Ogre::Vector3 position, Ogre::Vector3 actorPos, const Ogre::Matrix3 boneOffset = Ogre::Matrix3::IDENTITY);
};
class SkeletalActor : public Actor
{
public:
	int			skeletalID;		//for the physics engine dll
	
	ScytheSkeleton	skeleton;		//for rendering and moving
	ScytheBone*		rootBone;		

	virtual void		render();		//getting and setting positions
private:
};

#include "Anima_nsend.h"