#pragma once

class ANIMA_CORE_API GLUtil
{
private:

	static void DrawSphere (const float pos[3], const float R[12],float radius);

public:

	enum DrawType
	{
		Normal=0,
		Focus,
		Selected,
		Ghost
	};

	GLUtil(void);
	~GLUtil(void);

	static void DrawLine(Ogre::Vector3 &p1,Ogre::Vector3 &p2);
	static void DrawCube();
	static void DrawCubeAxis();

	static void DrawMarker(double x,double y,double z,double XZAngle,DrawType type);
	static void DrawPoint(double x,double y,double z,DrawType type);
	static void DrawPoint(double x,double y,double z,float size);

	static void setTransform (const float pos[3], const float R[12]);

	static void drawPatch (float p1[3], float p2[3], float p3[3], int level);
	static void DrawSphere();
	static void DrawSphere(float radius);
};
