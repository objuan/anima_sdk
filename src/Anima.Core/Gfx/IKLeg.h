#pragma once

#include "IK/VectorRn.h"

class GfxModel;
class Node;
class Tree;

class ClipNode;
class ModelGroupClip;

class IKLeg {

private:

	QList<Node*> nodes;
	Tree *tree;
	ClipNode *clipNodes[10];
	KFbxNode *leg_nodes[10];
	KFbxTakeNode *leg_takeNodes[10];
	KFbxVector4 leg_dir[10];
	KFbxXMatrix leg_trx[10][2];
	double leg_len[10];

	KFbxNode *lastNode;
	ModelGroupClip *lastModelGroupClip;

//	VectorR3 dir;
	VectorR3 dirX;
	VectorR3 dirY;
	VectorR3 dirZ;
	VectorR3 rotDir;

	int effectorNodeIdx;
	bool isLeft;
	int idx;
	KFCurve *Curve[10][9];
	//int rootNodeIdx;
	//int level;

	int firstNodeIdx;
	int lastNodeIdx;
	QList<KTime> keyTimes;

private:

	void ComputeLegMove(KTime time);
	VectorR3 GetFeetOffset(KTime time);

	void FillNodes(KFbxNode *node);
	void GetNodeTrx(KFbxNode* pNode,  KTime& pTime, KFbxXMatrix& pParentGlobalPosition);
	void ClearNodes(KFbxNode *node);

public:
	IKLeg(int rootNodeIdx,int level,bool isLeft);
	~IKLeg();

	//void SetFoot();
	bool Contains(int idx){return idx>= firstNodeIdx && idx <= lastNodeIdx;}
	void Set(KFbxNode *node,ModelGroupClip *modelGroupClip);

	void Copy(KFbxTakeNode *takeNode,int idx);

	void Draw(KTime time);
};

