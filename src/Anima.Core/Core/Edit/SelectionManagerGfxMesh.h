#pragma once

#include "Core/SceneObjectGfx.h"
//#include <OgrePanelOverlayElement.h>
//#include <OgreOverlayElementFactory.h> 
#include "Ogre/EntityMesh.h"

#include "Anima_nsbegin.h"

class DynamicLines;
class SceneObjectMovable;
class SplineMovePath;
class ClonerModder;
class MovePathManipolator;

// ========================= POINT ===========================================

class SelectionManagerGfxPointMover : public SubSceneObjectGfx 
{
private:

public:
	SelectionManagerGfxPointMover(SceneObjectGfx *obj);
};


class SelectionManagerGfxPointMoverMesh : public SceneObjectGfxMesh  
{
private:
	

public:
	SelectionManagerGfxPointMoverMesh(){}
	virtual ~SelectionManagerGfxPointMoverMesh(){}

	void Create(Ogre::String &name);
	void UpdateSelection();

	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_EDIT;}
};

// ============================================================

class SelectionManagerGfxManipolator: public SubSceneObjectGfx
{
protected:
	/*Ogre::Vector3 o;
	Ogre::Vector3 p;*/

public:

	SelectionManagerGfxManipolator(SceneObjectGfx *obj);
	virtual ~SelectionManagerGfxManipolator();

	//virtual void OnPreRenderTargetUpdate();

	void Update(MovePathManipolator *manipolator);

	virtual QString GetDesc(){return "SelectionManagerGfxMultiLines";}

	//virtual AObject::Type GetType(){return AObject::MovePathTranslateType;}
};

class SelectionManagerGfxMoveUIBound : public SubSceneObjectGfx
{
public:
	SceneObjectMovable *obj;
	Ogre::Vector3  O;
	Ogre::Vector3 off;
	
public:

	SelectionManagerGfxMoveUIBound(SceneObjectGfx *obj);
	virtual ~SelectionManagerGfxMoveUIBound();

	//virtual void OnPreRenderTargetUpdate();

	void Set(SceneObjectMovable *obj);
	void Move(Ogre::Vector3 &off){this->off=off;}
	
	virtual QString GetDesc(){return "SelectionManagerGfxMoveUI";}
	virtual AObject::Type GetType(){return AObject::SelectionManagerGfxMoveUIBoundType;}
};

class SelectionManagerGfxMoveUIAxis: public SubSceneObjectGfx
{
private:
	SceneObjectMovable	*obj;
public:
	
	SelectionManagerGfxMoveUIAxis(SceneObjectGfx *obj);
	virtual ~SelectionManagerGfxMoveUIAxis();

	void Set(SceneObjectMovable *obj);
	SceneObjectMovable *GetSceneObjectMovable(){return obj;}
	
	virtual QString GetDesc(){return "SelectionManagerGfxMoveUIAxis";}
	virtual AObject::Type GetType(){return AObject::SelectionManagerGfxMoveUIAxisType;}
};


// ====================

class SelectionManagerGfxMultiLinesMesh : public SceneObjectGfxMesh  
{
private:
	/*DynamicLines *base_line;
	DynamicLines *drag_line;
	DynamicLines *aus_line;*/
	Ogre::ManualObject *mo;

public:
	SelectionManagerGfxMultiLinesMesh();
	virtual ~SelectionManagerGfxMultiLinesMesh();

	Ogre::ManualObject *GetManualObject(){ return mo;}
	/*DynamicLines *GetBaseLines(){return base_line;}
	DynamicLines *GetDragLines(){return drag_line;}
	DynamicLines *GetAusLines(){return aus_line;}*/

	void Create(Ogre::String &name);
	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_EDIT;}
};


class SelectionManagerGfxLineMesh : public SceneObjectGfxMesh  
{
private:
	int numLines;
	int numPoints;
	QList<DynamicLines *> line_list;
//	DynamicLines *drag_line;


public:
	SelectionManagerGfxLineMesh(int numLines,int numPoints);
	virtual ~SelectionManagerGfxLineMesh();

	DynamicLines *GetLine(int idx){return line_list[idx];}
	//DynamicLines *GetBoundDragLine(){return drag_line;}

	void Create(Ogre::String &name);
	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_EDIT;}
	//void Update();
};

// ========================= CLONER ===========================================

class SelectionManagerGfxCloner : public SubSceneObjectGfx 
{
private:
	ClonerModder *cloner;

public:
	SelectionManagerGfxCloner(SceneObjectGfx *obj);
//	virtual ~SelectionManagerGfxCloner();

	void Set(ClonerModder *cloner);
};
class SelectionManagerGfxClonerMesh : public SceneObjectGfxMesh  
{
private:
	ClonerModder *cloner;
	Ogre::ManualObject *mo;
	int old_type;
	double old_v1;
	double old_v2;

public:
	SelectionManagerGfxClonerMesh();
	virtual ~SelectionManagerGfxClonerMesh();

	void Set(ClonerModder *cloner);
	void Create(Ogre::String &name);
	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_EDIT;}
};

class SelectionManagerGfxClonerFocus : public SubSceneObjectGfx 
{
private:
	ClonerModder *cloner;
	bool axeMode;

public:
	SelectionManagerGfxClonerFocus(SceneObjectGfx *obj,bool axeMode);

	ClonerModder *GetCloner(){return cloner;}
	void Set(ClonerModder *cloner);
		
	virtual QString GetDesc(){return "SelectionManagerGfxClonerFocus";}
	virtual AObject::Type GetType(){return AObject::SelectionManagerGfxClonerFocusType;}
};




class SelectionManagerGfxClonerFocusMesh: public SceneObjectGfxMesh  
{
private:
	ClonerModder *cloner;
	Ogre::ManualObject *mo;

public:
	SelectionManagerGfxClonerFocusMesh(SceneObject *);
	virtual ~SelectionManagerGfxClonerFocusMesh();

	void Set(ClonerModder *cloner);
	void Create(Ogre::String &name);
	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_EDIT;}
};




#include "Anima_nsend.h"