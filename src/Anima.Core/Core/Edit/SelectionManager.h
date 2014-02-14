#pragma once

#include "../Context.h"
#include <qrect.h>
#include "Anima.Core.h"
#include "Anima_nsbegin.h"

class FilmLayer;
class Animation;
class AnimationPart;
class ModelInstance;
class SceneObject;
class SelectionManagerGfx;
class SelectionRectangleOverlay;
class SceneObjectGfx;

class SelectionSceneObject : public BObject
{
public:
	SceneObject *Obj;
	int subIdx;
	//SelectionGroup *Parent;
	//QList<SelectionGroup *> Childs;

	//int Count(){return Childs.count();}
	//
	SelectionSceneObject() 
	{
		Obj=NULL;
		subIdx=-1;
	}
	SelectionSceneObject(SceneObject *Obj,int subIdx) : Obj(Obj) ,subIdx(subIdx)
	{
		//Parent=NULL;
	}
        ~SelectionSceneObject(void)
	{
		//ClearChilds();
	}

	//bool IsLeaf(){return Childs.count() == 0;}
	//void ClearChilds();
	//bool RemoveChild(SelectionGroup *child);
	//SelectionGroup *Find(SceneObject *obj);
	//void GetList(AObject::Type *type,QList<SelectionGroup *> &list,bool typeEquals=true);
};

class SceneObjectDAG : public BObject
{
public:
	SceneObject *Obj;
	int subIdx;
	SceneObjectDAG(SceneObject *Obj,int subIdx) : Obj(Obj),subIdx(subIdx)
	{
	}
};

class SceneObjectEditInfo : public BObject
{
public:
	Ogre::Vector3 firstPoint;
	float firstAngle;
	Ogre::Vector3 actualPoint;
	SceneObjectGfx *obj;
	SceneObjectGfx *sub_obj;
	QList<SceneObjectGfx *> dependenceObjList;
};

class ANIMA_CORE_API SelectionManager: public QObject ,public BObject
{
	Q_OBJECT
public:

	enum SelectionMode
	{
		Exclusive,
		BeginAdditive,
		Additive,
		Range
	};

	static SelectionManager *Singleton;

private:
	bool lock;
	SceneObject *copyObj;
	QList<SelectionSceneObject *> selectedObj;
	SelectionManagerGfx *selectionManagerGfx;
    SelectionRectangleOverlay * selectionRectangle;

	QList<SceneObject *> selectedPathList;

	bool fireSelectionChanged;
	bool editEnabled;
	SceneObjectEditInfo objectEdit;
	QRectF selectionRectange;

private:

	void OnLastEditModeChanged();

public:

	SelectionManager();
	~SelectionManager();

	void Init();
	void InitUI();
	void Clear();
	void SetLock(bool lock);
	SelectionManagerGfx *GetSelectionManagerGfx(){return selectionManagerGfx;}


	QRectF &GetSelectionRectange(){return selectionRectange;}

	// copy & paste
	void SetCopyObj(SceneObject *copyObj){this->copyObj=copyObj;}
	void ApplyCopy(SceneObject *toObj);
	bool CanCopy(SceneObject *toObj);
	SceneObject *GetCopyObject(){return copyObj;}

	void Select(SceneObject *obj,int subObjectIdx=-1,SelectionMode mode=SelectionManager::Exclusive);
	bool DoSelect(SceneObject *obj,int subObjectIdx,SelectionMode mode);

	void Remove(SceneObject *obj);

	// query
	bool IsSelected(SceneObject *obj,int subObjectIdx=-1);
	
	SceneObject *GetFirstSelect();
	SceneObject *GetFirstSelect(AObject::Type type,bool typeEquals,int subObjectIdx); // filtro tipo
	SceneObject *GetFirstSelect(AObject::Type type,bool typeEquals=false); // filtro tipo
	
	bool IsEditing(SceneObjectGfx *gfx);

	// edit
	void BeginObjectEdit(SceneObjectGfx *obj,const Ogre::Vector3 &firstPoint,float firstAngle);
	void ChangeEditPoint(Ogre::Vector3 &p);
	void EndObjectEdit();
	bool IsObjectEditEnabled(){return editEnabled;}
	SceneObjectEditInfo GetSceneObjectEditInfo(){return objectEdit;}
	void BeginSubObjectEdit(SceneObjectGfx *obj);
	void EndSubObjectEdit();
	void NotifyDependenceObject(SceneObjectGfx *obj);

	// casi
	// muovo un punto del path con il mouse
	// sono in new
	//bool CanAnimationPartReceiveEvents(AnimationPart *part);

	// selection rectangle

	void BeginSelectionRect(float x,float y);
	void ChangeSelectionRect(float x,float y);
	bool EndSelectionRect(QRectF &selectionRectange);

	// ==============

	void InvalidateGfx();
	void OnEditModeChanged();
	void FlushEvents();

	// tengo l'ordine
	void GetSelectGroup(AObject::Type type,QList<SceneObject *> &group,bool typeEquals=false);

signals:
	void SelectionChanged();

private slots:

	//void OnRemoveFilmLayer(FilmLayer *filmLayer);
	//void OnRemoveAnimation(Animation *anim);
	//void OnRemoveAnimationPart(AnimationPart *part);

};


#include "Anima_nsend.h"
