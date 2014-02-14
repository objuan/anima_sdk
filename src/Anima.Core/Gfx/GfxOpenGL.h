#pragma once

class ANIMA_CORE_API GfxOpenGL
{
private:


public:

	enum DrawType
	{
		Normal=0,
		Focus,
		Selected,
		Ghost
	};

	GfxOpenGL(void);
	~GfxOpenGL(void);

	static void DrawCube();
	static void DrawCubeAxis();

	static double Angle2DDeg(double dx,double dy);
	static double Angle2DRad(double dx,double dy);

	static void DrawMarker(double x,double y,double z,double XZAngle,DrawType type);
	static void DrawPoint(double x,double y,double z,DrawType type);
};
