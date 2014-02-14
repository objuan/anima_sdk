#pragma once

#include "ModelClip.h"

#include "Anima_nsbegin.h"

class ClipBrainAssign: public BObject
{
public:
	ModelClip *clip;
	int clipCount;
	double clipSpace;
	double cutClipTime;
	int clipIdx;

	bool isResize;
	double IKFactor; // 0 = no, 1 = ai limiti min - max
	double Quality; // 0 = better

	ClipBrainAssign(ModelClip *clip,int clipCount,double size,int clipIdx);
};


class ClipBrainSolution: public BObject
{
public:
	double Quality; // 0 ok, 1 divergente
	QList<ClipBrainAssign *> assignList;

	ClipBrainSolution():Quality(0){}
	~ClipBrainSolution();
	ClipBrainSolution *Copy();
	void Add(ClipBrainAssign *ass);
	void Dump();

};

class ClipBrainEntry: public BObject
{
public:
	double space;
	QList<ClipBrainAssign *> assignList;
};

class ClipBrainModelEntry: public BObject
{
public:
	int max_space_idx;
	double max_space;
	double max_space_out;
	double min_space;
	QList<ClipBrainEntry *> entryList;
};

class ANIMA_CORE_API ClipBrain : public BObject
{
private:
	ModelClip *base_clip;
	QList<ModelClip *> clipList;

	QList<ClipBrainSolution *> solutionList;
	QList<ClipBrainSolution *> __solutionList;
	QList<ClipBrainAssign *> __assignList;
	QList<ClipBrainAssign *> assignList;

	bool newMode;
	static QMap<QString,ClipBrainModelEntry *> *_assignMap;

private:
	void _AssignClips(QList<ClipBrainEntry *> &entryList,double space,int mult,double sp_min ,double sp_max,QList<ClipBrainAssign *> &assignList );
	bool _Assign(ClipBrainEntry *entry,double space,int mult,QList<ClipBrainAssign *> &assignList);
	void _DecideAll(double space,bool forceOld);
	void _Decide(ModelClip *clip,int idx,double freeSpace,ClipBrainSolution *solution);
	bool Eval();

public:

	ClipBrain(ModelClip *clip);
	virtual ~ClipBrain();
	void Clear();
	bool DecideAll(double space,double firstClipTime);
	QList<ClipBrainAssign *> &GetAssign(){return assignList;}

	// statiche di inizializzazione
	static void Init(); // load
	static void Alloc();
	static void End();

	static void AddPreset(ModelClip *clip,double maxSpace,double stepSpace);
	static void Save();
	static bool Load(Model *model ,const QString &serial,bool onlyCheck);

	//void Dump();
};
#include "Anima_nsend.h"