#pragma once
#include "Core/IA/ModelIAActor.h"

#include "Anima_nsbegin.h"

class ModelInstance;
class GfxClipAnim;
class MovePath;
class AnimClipModel;
//class ModelInstanceIAPath;
class ModelIAThread;
class ModelIABuilderPath;
class Animation;
class ModelIAActor;
class ModelIAFrame;
class ModelIACollisionManager;
class CollisionActor;
class ModelIAActorDecisionSpeedFunc;
class QRandom;
class ModelIAActorDecisionBrainNode;
class QRandom;;
class ModelIAActorDecisionBrainEval;
class ModelIAActorDecisionHistoryDecisionList;
class ModelIAActorDecisionBrainNode;

class VariableContext: public BObject
{
public:
	/*class VariableContextEntry
	{
	public:
		int assigned;
		int intVal;
		float floatVal;
		QString stringVal;
		VariableContextEntry():assigned(0),intVal(0),floatVal(0),stringVal(""){}
	};
	QMap<QString,VariableContextEntry*> entryList;
public:
	int actor1Assigned;
	int actor2Assigned;
	int lastActorOKTested;
public:
	VariableContext():actor1Assigned(0),actor2Assigned(0),lastActorOKTested(0){}
	virtual ~VariableContext();
	void Add(const QString &name,int actor,int value);
	void Add(const QString &name,int actor,float value);
	void Add(const QString &name,int actor,const QString &value);
	bool GetValue(const QString &name,int actor,int &intVal,float &floatVal,QString &stringVal);*/

public:
	int actor1Assigned;
	int actor2Assigned;

	VariableContext(int m1ActorIdx):actor1Assigned((m1ActorIdx == 1) ? 1 : 2),actor2Assigned((m1ActorIdx == 1) ? 2 : 1){}
};

class PredicateNode: public BObject
{
public:
	PredicateNode(){}
	virtual void GetActorCond(int actorIdx,QStringList &condList)=0;
	virtual bool Eval(const QString &varName,VariableContext &context)=0;
	virtual QString ToString()=0;
};
	
class PredicateVariable : public PredicateNode
{
private:
	 QString svar;
	
	 QStringList stringVarList;
	 int intVal;
	 int type; // 0 = int , 1 = float, 2 = string,3 range

	 int actorIdx;
	 bool invert;

public:

	PredicateVariable();
	PredicateVariable(const QString &var);
	virtual void GetActorCond(int actorIdx,QStringList &condList);
	virtual bool Eval(const QString &varName,VariableContext &context);
	virtual QString ToString(){return svar; }
};

class PredicateVariableOperator : public PredicateNode
{
private:
	//int turn;
	PredicateVariableOperator *parent;
public:
	QList<PredicateNode *> varList;
	int operation;

	PredicateVariableOperator(PredicateVariableOperator *parent):parent(parent){}
	void Add(const QString &var); 
	void Close();
	virtual void GetActorCond(int actorIdx,QStringList &condList);
	virtual bool Eval(const QString &varName,VariableContext &context);
	virtual QString ToString();
};

class ANIMA_CORE_API ModelIAActorDecisionBrainNodeConditionPredicate: public BObject
{
private:
	PredicateNode *mainVar;
public:
	QString cond;
public:
	QString GetCond(){return cond;}
	void GetActorCond(int actorIdx,QStringList &condList);
	ModelIAActorDecisionBrainNodeConditionPredicate(const QString &cond);
	bool Eval(const QString &varName,VariableContext &context);
};
class ANIMA_CORE_API ModelIAActorDecisionBrainNodeCondition: public BObject
{
private:
	ModelIAActorDecisionBrainNodeConditionPredicate *sideEnter;
	ModelIAActorDecisionBrainNodeConditionPredicate *sideIn;
	ModelIAActorDecisionBrainNodeConditionPredicate *sideExit;
	float time_min;
	float time_max;
	int speed_m1_min_m2; // -1= disabled = 1 = YES , 0 = NO
	//float speed_factor_max;
	float hit_angle_enter_min;
	float hit_angle_enter_max;
	int  hit_angle_enter_mode;
	float hit_angle_exit_min;
	float hit_angle_exit_max;
	int  hit_angle_exit_mode;
	float path_angle_min;
	float path_angle_max;
	int  path_angle_mode;

public:
	ModelIAActorDecisionBrainNodeCondition(QDomElement ele,QStringList &m1Assign,QStringList &m2Assign);
	virtual ~ModelIAActorDecisionBrainNodeCondition();
	ModelIAActorDecisionBrainNodeConditionPredicate *GetSideEnter(){return sideEnter;}
	ModelIAActorDecisionBrainNodeConditionPredicate *GetSideExit(){return sideExit;}
	bool Eval(ModelIAActorDecisionBrainEval *eval);
	void PostLoad();
};

class ANIMA_CORE_API ModelIAActorDecisionBrainNodeLink: public BObject
{
private:
	QString _starget;
	int outActor;
	ModelIAActorDecisionBrainNode *target;
	QList<ModelIAActorDecisionBrainNodeCondition *> condList;
public:
	
	ModelIAActorDecisionBrainNodeLink(){}
	virtual ~ModelIAActorDecisionBrainNodeLink(void);

	QString GetTarget(){ return _starget; }
	QList<ModelIAActorDecisionBrainNodeCondition *> GetConditions(){ return condList;}

	virtual void Load(QDomElement e);
	void PostLoad();
	bool Eval(ModelIAActorDecisionBrainEval *eval);
	ModelIAActorDecisionBrainNode *EvalOne(ModelIAActorDecisionBrainEval *eval);
};

struct ModelIAActorDecisionBrainNodeDecisionTarget: public BObject
{
	QString clipID;
	float time;
	float hit_space_offset;
	float hit_clip_time;
	QString target;
	ModelIAActorDecisionBrainNodeDecisionTarget(const QString &target);
};
class ANIMA_CORE_API ModelIAActorDecisionBrainNodeDecision: public BObject
{
public:
	ModelIAActorDecisionBrainNode *Node;
	bool Enabled;
	int outActor;
	int Priority;
	QString _sTarget;
	QString _sOtherTarget;
	QString FadeActor;
	QList<ModelIAActorDecisionBrainNodeDecisionTarget> targetList;
	QList<ModelIAActorDecisionBrainNodeDecisionTarget> targetOtherList;

	QList<ModelIAActorDecisionBrainNodeCondition *> condList;

	ModelIAActorDecisionBrainNodeDecision(ModelIAActorDecisionBrainNode *node):Node(node){}
	virtual ~ModelIAActorDecisionBrainNodeDecision(void);

	virtual void Load(QDomElement e);
	void PostLoad();
	bool Eval(ModelIAActorDecisionBrainEval *eval);
	bool HaveAttempt(int actorIdx,const QString &lastTarget);
	ModelIAActorDecisionBrainNodeDecisionTarget *NextAttempt(int actorIdx,const QString &lastTarget);
};

class ANIMA_CORE_API ModelIAActorDecisionBrainNode: public BObject
{
private:
	QList<ModelIAActorDecisionBrainNodeLink *> linkList;
	QList<ModelIAActorDecisionBrainNodeDecision *> decisionList;
public:
	QString ID;
	bool Enabled;

	ModelIAActorDecisionBrainNode(){}
	virtual ~ModelIAActorDecisionBrainNode(void);

	QList<ModelIAActorDecisionBrainNodeLink *>  &GetLinks(){return linkList;}
	QList<ModelIAActorDecisionBrainNodeDecision *>  &GetDecisions(){return decisionList;}

	virtual void Load(QDomElement e);
	void PostLoad();
	void Eval(ModelIAActorDecisionBrainEval *eval);
	ModelIAActorDecisionBrainNode *EvalOne(ModelIAActorDecisionBrainEval *eval);
};

class ModelIAActorDecisionBrainEvalContext: public BObject
{
public:
	ModelIAActorDecisionBrainEvalContext *parent;

	// out
	int outActorAssigned;
	ModelIAActorDecisionBrainNodeDecisionTarget *decide;

	//int outActorOther;
	ModelIAActorDecisionBrainNodeDecisionTarget *decideOther;

	bool enabled;

	ModelIAActorDecisionBrainNodeDecision *decisionNode;
	ModelIAActorDecisionBrainEvalContext();
	ModelIAActorDecisionBrainEvalContext(ModelIAActorDecisionBrainEvalContext &ctx);

};

class ModelIAActorDecisionBrainEval: public BObject
{
public:
	ModelIAActorCollision *coll;

	ModelIAActorDecisionBrainEvalContext *currentContext;
	QList<ModelIAActorDecisionBrainEvalContext *> outContextList;

	// fixed variables
	ModelIAActorDecisionHistoryDecisionList *actorDecHistory;
	ModelIAActorDecisionHistoryDecisionList *otherDecHistory;

	VariableContext varAssigned;
	
	ModelIAActorDecisionBrainEval(int m1ActorIdx);
	virtual ~ModelIAActorDecisionBrainEval();

	void PushContext();
	void PopContext();
	ModelIAActorDecisionBrainEvalContext *CurrentContext(){return currentContext;}

	void AddOutContext();
};

class ANIMA_CORE_API ModelIAActorDecisionBrain  : public BObject
{
private:

	static int randomSeed;
	static QRandom *rnd;
	static ModelIAActorDecisionBrainNode *StartNode;
	static ModelIAActorDecisionBrainNode *DefaultNode;
	static QMap<QString,ModelIAActorDecisionBrainNode *> *brainNodeList;
public:
	static ModelIAActorCollision *currentColl;

	static void AddSolution(ModelIAActorDecision *decision ,ModelIAActorCollision *coll,ModelIAActorDecisionBrainEval &eval);
public:

	static void Init();
	static void End();
	static void StartBuild();
	static ModelIAActorDecision* DecideHistory(ModelIAActorCollision *coll,ModelIAActorDecisionBrainEval *eval);
	static ModelIAActorDecision* Decide(ModelIAActorCollision *coll);

	static void Clear();
	static void LoadAll(QDomElement e);
	static ModelIAActorDecisionBrainNode *Find(const QString &ID);

	static QRandom *GetRandom(){return rnd;}
};


#include "Anima_nsend.h"
