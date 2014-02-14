#pragma once

#include "Action.h"
#include "Anima_nsbegin.h"

class Action;
class Model;

class ANIMA_CORE_API ActionLibrary : public BObject
{
private:

	QHash<QString,Action *> actionMap;
	QList<Action *> actionList;
    QList<Action *> actionClipList;
   
public:
	ActionLibrary(void);
	virtual ~ActionLibrary(void);

	void Load();

	Action *Find(const QString &actionName);

	void GetEnabledActions(QList<Action *> &list,const QString &category,Action::ActionType type);
	void GetTargetActions(SceneObject *obj,Action::ActionType type,QList<Action *> &actions);

	void GetSourceActions(Model *model,QList<Action *> &actions);

	void GetActionClipsCategoryList(QList<QString> &catList);
	QList<Action *> &GetActionClips(){ return actionClipList;}
};

#include "Anima_nsend.h"