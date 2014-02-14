#pragma once

class Body;

//#include "Vector.h"

//class Link;
//class Joint;
class Bone;
class Marker;
//class IKChain;
//class ArticulatedObject;

class BoundingBox;

class SkeletonChain
{
public:
	QString name;
	QString effectorName;
	QList<Bone *> boneList;
	bool effector;
	SkeletonChain(QString &name) : name(name){}
};

class Skeleton {
private:

	Body *body;
//	ArticulatedObject *articulatedObject;
	QHash<QString,SkeletonChain *> chainMap;

//	int joinIdx;
//	int linkIdx;

private:

	void Create(Bone *bone,int keyIdx);
	void Create(int keyIdx);

public:
	Skeleton(Body *body);
	~Skeleton();

	void AddChain(QDomElement &chainEle);

	//void LoadKey(int keyIdx);
	//void MoveChain(const QString &name,KFbxXMatrix &trx);

	//void Draw(int keyIdx);
};

