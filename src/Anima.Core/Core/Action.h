#pragma once

#include "Context.h"
#include "./Item.h"

#include "Anima_nsbegin.h"

class Model;
class Clip;
class AnimClip;
class Command;

class ANIMA_CORE_API Action : public Item
{
public:

	enum ActionType
	{
		ActionClip,
		ActionAnimation_AddPart,
		ActionActorModder
	};

private:
	ItemCategoryCollection category;
	QColor color;
	QString name;
	QString desc;
	QList<AObject::Type> targetList;
	QList<Clip *> clipCondition;

protected:

	AnimatedImage image;

	void AddType(AObject::Type type);

public:
	Action(void);
	virtual ~Action(void);

	QString &GetName(){return name;}
	QString &GetDesc(){return desc;}
	QColor &GetColor(){return color;}
	ItemCategoryCollection &GetCategory(){return category;}
	AnimatedImage &GetImage(){return image;}
	virtual ItemType GetItemType(){return ItemType_Action;}

	QList<AObject::Type> &GetTargetTypeList(){return targetList;};
	//void GetTargetTypeList(QList<AObject::Type> &list);
	virtual void Load(QDomElement e);

	virtual bool CanTarget(SceneObject *obj);
	bool CanSource(Model *model);

	virtual ActionType GetType()=0;
	virtual void *Execute(Command *cmd,SceneObject *obj,int pos)=0;
	virtual void Undo(Command *cmd,SceneObject *obj,int pos,void *tag)=0;
	virtual void Exit(Command *cmd){}
	virtual void Abort(Command *cmd){}

	virtual void OnMouseDown(Command *cmd,MouseCollisionEvent &ev){}
	virtual void OnMouseUp(Command *cmd,MouseCollisionEvent &ev){}
	virtual void OnMouseDrag(Command *cmd,MouseCollisionEvent &ev){}
	virtual void OnMouseMove(Command *cmd,MouseCollisionEvent &ev){}
	virtual bool IsPathCommand(){return false;}
};
#include "Anima_nsend.h"

