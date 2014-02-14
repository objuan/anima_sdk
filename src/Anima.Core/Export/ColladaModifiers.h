/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America
	
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#ifndef COLLADAMODIFIERS_
#define COLLADAMODIFIERS_

//#include "IMorpher/IMorpher.h"
//#include "CS/PhyExp.h"
#include <map>
#include <vector>

class Modifier;
class Object;
class IDerivedObject;
class ColladaSkinInterface;
class ISkinModInterface;

typedef int Class_ID;

#define SKIN_CLASSID 1
#define MORPHER_CLASS_ID 2
#define PHYSIQUE_CLASS_ID_A 3
#define PHYSIQUE_CLASS_ID_B 4
#define COLLADA_SKIN_CLASS_ID		SKIN_CLASSID
#define COLLADA_MORPHER_CLASS_ID	MORPHER_CLASS_ID
#define PHYSIQUE_CLASSID Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B)

class ColladaModifier
{
protected:
	int modifierIndex;
	int modifierStackIndex;
	//IDerivedObject* object;
	bool resolved;

public:
	ColladaModifier() : modifierIndex(0), resolved(false) {}
	ColladaModifier(const ColladaModifier& copy) { modifierIndex = copy.modifierIndex;  resolved = copy.resolved; }
	virtual ~ColladaModifier(){}

	virtual Modifier* GetModifier();
	virtual int GetModifierIndex() { return modifierIndex; }
	virtual int GetModifierStackIndex() { return modifierStackIndex; }

	virtual Class_ID GetClassID() = 0;
	virtual int Matches(Modifier* modifier) = 0;
	virtual void Clear(){ modifierIndex = 0; resolved = false; }
	virtual bool IsResolved() { return resolved; }
	//virtual void Resolve(Object* obj);
	//virtual IDerivedObject* GetDerivedObject() { return object; }
	//virtual Object* GetInitialPose();
};

class ColladaSkin : public ColladaModifier
{
public:
	ColladaSkin() : ColladaModifier(){ }
	ColladaSkin(const ColladaSkin& skin) : ColladaModifier(skin) { }
	virtual ~ColladaSkin(){};
	virtual Class_ID GetClassID() { return COLLADA_SKIN_CLASS_ID; }
	virtual int Matches(Modifier* modifier);
	ColladaSkinInterface* GetInterface(Ogre::Mesh* mesh);
};

//
// Abstract wrapper class defines all the functionality we will want from 
// whatever skin classes we will work with.  To implement an
// interface for whatever skinnable modifier, inherit this class
// and implement its functinos
//
class ColladaSkinInterface
{
public:
	/* Call this function when you are done with this interface
	*  to release the skins back into the wild.  Will also delete
	*  this (the current class) so dont touch it again. */
	virtual void ReleaseMe()=0;

	/* Get the transform of the ojbect
	*  when it was skinned */
	virtual bool GetSkinInitTM(Ogre::Matrix3 &initTM, bool bObjOffset = false)=0;

	/* Get the initial transform of the
	*  i'th bone when skinned */
	virtual bool GetBoneInitTM(Ogre::Bone* node, Ogre::Matrix3 &boneInitTM)=0;

	/* Get the number of bones affecting this skin */
	virtual int GetNumBones()=0;

	/* Get the i'th bone */
	virtual Ogre::Bone* GetBone(int i)=0;

	/** Vertex Methods* */

	/* Get the number of vertices' this
	*  modifier affects */
	virtual int GetNumVertices()=0;

	/* Get the number of bones assigned
	*  to the i'th vertex */
	virtual int GetNumAssignedBones(int i)=0;

	/* Get the bone wieght of the bindex
	*  bone on the vindex'ed vertex */
	virtual float GetBoneWeight(int vertexIdx, int boneIdx)=0;

	/* Get the Joint index of the bindex bone on
	*  the vindex'ed vertex */
	virtual int GetAssignedBone(int vertexIdx, int boneIdx)=0;
};

/*
*  Skin modifier wrapper
*  CAUTION - This should only be used for the node its
*  instantiated for.
*/

class ISkinModInterface : ColladaSkinInterface
{
private:
	Ogre::Mesh* node;
	Modifier* m;
	//ISkin* modInterface;
	//ISkinContextData* context;

public:
	ISkinModInterface(Modifier* m ,Ogre::Mesh* mesh);

	~ISkinModInterface();
	void ReleaseMe();

	bool GetSkinInitTM(Ogre::Matrix3 &initTM, bool bObjOffset = false);
	bool GetBoneInitTM(Ogre::Bone* node, Ogre::Matrix3 &boneInitTM);

	int GetNumBones();
	Ogre::Bone* GetBone(int i);

	int GetNumVertices();
	int GetNumAssignedBones(int i);
	float GetBoneWeight(int vertexIdx, int boneIdx);
	int GetAssignedBone(int vertexIdx, int boneIdx);
};
#endif
