#pragma once
#include "Anima_nsbegin.h"
//
////primitives (can also be a model, light or joint)
//class EdPrimitive : public Ogre::UserDefinedObject
//{
//public:
//	EdPrimitive();
//	~EdPrimitive();
//
//	virtual long getTypeID()
//	{ return CLASS_PRIMITIVE; }
//
//	void				setup(int type, Ogre::Entity* ent);
//	int				id;
//	Ogre::Entity*		entity;
//	Ogre::Light*		light;
//	Ogre::Vector3			pos;
//	Ogre::Vector3			size;			//[2] not used for LSS
//	int				type;			//eg EDTYPE_BOX, EDTYPE_JOINT etc
//	//int				primitiveType;		//eg PRIMITIVE_BOX, JOINT_BALL
//	int				model;			//should hold primitive type eg box
//	int				materialIndex;
//	Ogre::String			name;				//same as actor name, FOR JOINTS ONLY
//
//	//for skeletal models
//	ScytheSkeleton*		skeleton;
//
//	//if its just a bone
//	ScytheBone*			boneptr;
//
//	//collision mesh data
//	bool				usesPreCookedMesh;
//	Ogre::String		meshFile;
//	Ogre::String		cookedFile;
//	bool				convex;
//	bool				smooth;
//	bool				doubleSided;
//
//	//vec3_t		spheres[2];	//position of spheres on ends of cylinder to create a LSS (capsule)
//	///rotation;
//	Ogre::Vector3				com;		//center of mass
//	Ogre::SceneNode*		node;
//	Ogre::Quaternion		getOrientation();
//
//	//collision for selecting
//	bool		selected;	
////	box_t		box;
//	void		updateScanCollision();
//	
//	//lights only
//	Ogre::Vector3	RGB;		//for lights - diffuse
//	Ogre::Vector3	specular;	
//	float		range;		//lights range
//	//joints only
//	EdActor*	joinedActors[2];		//the 2 actors we've joined
//	float		limits[4];			//4 joint limits
//	bool		limitsOn;
//	bool		selfCollision;
//	float		breakable[2];
//	//
//	int				projectionMode;
//	float			projectionDist;		//only hinge, ball and 6DOF
//	float			projectionAngle;
//	//hinge only
//	PhysicsMotor	motor;
//	//hinge + ball
//	PhysicsSpring	twist;
//	//ball
//	PhysicsSpring	swing;
//	PhysicsSpring	dist;
//	//
//	float			stiffness;
//	
//	//phys properties
//	//primitives only
//	float		mass;
//	float		linearDamping;
//	float		angularDanping;
//	float		maxAngVel;
//	int			group;
//	float		density;
//	float		skinWidth;
//	int			maxIterations;
//
//	float		customValue[8];
//	//
//	EdActor*	attachedToActor;	//index of actor me attached to			//-1
//	
//	//MODELS ONLY
//	std::string		modelName;	//model name
//	std::string		materialName;
//
//	void		select();		//select this primitive
//	void		flash();			//makes the object flash, for indications
//	int			flashCounter;
//	void		detach();
//
//	Ogre::Entity*	jointParts[4];	//joint links and limits
//
//	//update pos, orientation,
//	void		renderJoint(Ogre::SceneNode* node);
//	void		renderPrimitive(Ogre::SceneNode* node);
//	void		renderLight(Ogre::SceneNode* node);
//	void		renderModel(Ogre::SceneNode* node);
//	//update materials
//	void		updateJointMaterials(int renderMode);
//	void		updatePrimitiveMaterials(int renderMode);
//	void		updateModelMaterials(int renderMode);
//	void		updateLightMaterials(int renderMode);
//
//	bool		isPlaceholder;			//indicates the proper model could not be found
//	bool		frozen;
//
//	bool			triggerOnEnter;
//	bool			triggerOnStay;
//	bool			triggerOnLeave;
//	bool			fluidDrain;
//private:
//	
//};
//

#include "Anima_nsend.h"