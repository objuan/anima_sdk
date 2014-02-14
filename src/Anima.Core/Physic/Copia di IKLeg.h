#pragma once

#include "IK/VectorRn.h"

class GfxModel;
class Node;
class Tree;

class ClipNode;
class Body;
class Bone;
class IKLeg;

class IKLeg {

private:

	Body *body;
	Bone *rootBone;
	Bone *target1;
	Bone *target2;

	QList<Node*> nodes;
	Tree *tree;
	int level;

	VectorR3 dirX;
	VectorR3 dirY;
	VectorR3 dirZ;
	VectorR3 rotDir;

	QList<KFbxVector4> rotSeq;
	//QList<int> nodeIdxSeq;

	int effectorNodeIdx;
	int effectorNodeIdxToe;
	bool isLeft;

	int idx;
	int node_idx;

	//int firstNodeIdx;
	//int lastNodeIdx;
	//Traker *traker;

private:

	//VectorR3 GetFeetOffset(KTime time);

	void BuildRot(KTime &time);

public:
	IKLeg(Body *body,Bone *rootBone,bool isLeftt);
	~IKLeg();

	void LoadFromBody();
	void SaveToBody();

	void Draw(KTime time);
};

