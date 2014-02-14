#pragma once

#include "./Command.h"
#include "Core/Edit/EditorManager.h"
#include "Core/Edit/SelectionManager.h"

#include "Anima_nsbegin.h"
class FilmLayer;
class Animation;
class Model;
class AnimationPartClipSpline;
class Clip;
class IMesh;
class SceneObject;
class AnimClip;
class MovePathPoint;
class Action;
class MovePath;
class ModelInstance;
class Modder;

class ANIMA_CORE_API CommandManager: public QObject 
{
	Q_OBJECT

public:
	static CommandManager Singleton;

private:

	CommandLogRowList logHistory;
	QList<Command *> cmdRedo;
	QList<Command *> cmdHistory;
	QStringList commandList;

	Command *currentCmd;
	ProgressBar *lastProgressBar;
	
private:
	
	void Abort();
	void Undo();
	void Redo();
	void Help();
	
public:
	CommandManager(void);
	virtual ~CommandManager(void);
	void Init();
	void InitUI();
	void End();

	Command *GetCurrentCmd(){return currentCmd;}
	CommandLogRowList &GetLogHistory(){return logHistory;}

	bool HaveUndo(){return cmdHistory.count() > 0;}
	bool HaveRedo(){return cmdRedo.count() > 0;}
	
	void AddCommand(const QString &cmd);
	int ExecuteScript(const QString &script);

	void FireCommandLog(CommandLogRowList &log); // log del comando
	void FireCommandLog(const QString &cmdLog,CommandLogType type); // log interno

	void CommandDone(Command *cmd);

	bool IsBusy();
	
	// low level commands

	// main

	void Load(const QString &path="");
	void Save(const QString &path="",bool force=false);

	void ImportScene(const QString &path="");

	// model
	void ImportMesh();
	void ImportMesh(const QString &filePath,UnitMeter unit);

	// edit
	//void SetEditOperationMode(EditorManager::OperationMode mode);
	void SetEditPickMode(Editor::PickMode mode);
	void EditPushMode();
	void EditPopMode();
	void LockEditMode(bool lock);
	void LockSelectionMode(bool lock);
	void Select(SceneObject *obj,int subObjectIdx,SelectionManager::SelectionMode mode = SelectionManager::Exclusive);

	// 
	void ExitCommand();

	// scene object 
	void Delete(SceneObject *sceneObject);
	void Rename(SceneObject *sceneObject);

	// dir = "" , "FB", "LR";
	void Transform(SceneObject *obj,Ogre::Vector3 pos,const QString &mode);

	// film layer
	void AddFilmLayer();

	// animation
	void AddAnimation(FilmLayer *filmLayer);
	void MoveAnimPartPos(AnimationPart *animPart,AnimationPart *afterPart);
	void InvertAnimation(Animation *anim);

	// gfx

	//void BeginMove(SceneObjectMovable *obj);
	void MoveUI(SceneObject *obj,const QString &mode);

	// model
	void AddActor(Animation *anim,QString &name);
	void ReplaceActor(ModelInstance *anim,QString &name);

	// action

	void DoAction(SceneObject *obj,Action *action);
	void DoAction(SceneObject *obj,Action *action,int pos);

	// clip

	//void MoveAnimClip(AnimClip *animClip,double startOffset,double endOffset);
	//void MoveAnimClipPos(AnimClip *animClip,AnimClip *afterClip);

	void OpenProperty(SceneObject *sceneObject);

	// path

	// aggiunge un punto alla selezione
	void AddPoint(Ogre::Vector3 pos,int idx=-1);
	void MoveSelection(Ogre::Vector3 pos);
	void RotateSelection(Ogre::Vector3 pos);
	void TranslatePath(MovePath *path,Ogre::Vector3 startPos);
	void RotatePath(MovePath *path,Ogre::Vector3 startPos);

	// dir = "" , "FB", "LR";
	void MoveModel(ModelInstance *model,Ogre::Vector3 pos,const QString &dir);
	void MoveModel(ModelInstance *model,Ogre::Vector3 pos,MovePath *path,double spaceDist,const QString &dir);


	// copy & paste

	void Copy(SceneObject *sceneObject);
	void Paste(SceneObject *sceneObject);
	void Clone(SceneObject *sceneObject);

	// merge /split
	/*void Merge(SceneObject *sceneObject,int mergeOffset);
	void Split(SceneObject *sceneObject);*/

	// offset

	void ModelOffsetClear(ModelInstance *,bool clearStartEnd,bool clearMarkers);

	// TODO
	//void ModelSpeedClear(ModelInstance *,int clipIdx,int mode ); // 0 = all , clip , marker
	//void ModelSpeedDeleteMark(ModelInstance *,QList<int> markList);
	//void ModelSpeedAddMark(ModelInstance *,int clipIdx,double spacePrec,double val);
	////void ModelSpeedMove(ModelInstance *,int x,int y);

	//void ModelSpeedBeginUpdate(ModelInstance *model);
	//void ModelSpeedEndUpdate(ModelInstance *model);

	// modders
	void StartChangeModder(Modder *modder);
	void EndChangeModder(Modder *modder);

	// property
	void StartChangeProperty(SceneObject *so,const QString &propName);
	void EndChangeProperty(SceneObject *so);

	// GUI events

	void EventAddModel(IMesh *mesh);
	void EventSelectActor(Model *model);
	void EventSelectClip(Clip *clip);

	void KeyPressEvent(class QKeyEvent *);
	
signals:

	void CommandStart(Command *cmd);
	void CommandExecuted(Command *cmd);
	void CommandLog(CommandLogRowList log); 

//private slots:

};
#include "Anima_nsend.h"
