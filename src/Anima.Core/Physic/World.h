#pragma once

class Body;

//#include "ode/ode.h"


#define MIN_DISTANCE 1
#define MAX_ITERATIONS 100
/*
class World {
friend class Bone;

public:

	dWorldID worldID;

	QList<Body *> bodyList;

private:
	World();
	~World();

public:

	static World Singleton;

	void Clear();
	void AddBody(Body *body);

	void MoveToPose();
};
*/
class World {
friend class Bone;

public:

	QList<Body *> bodyList;

private:
	World();
	~World();

public:

	static World Singleton;

	void Clear();
	void AddBody(Body *body);

	void MoveToPose();
};


