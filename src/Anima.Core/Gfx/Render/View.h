#pragma once

#include "Anima_nsbegin.h"

enum	{ L, R, T, RT };		//anchor position Left Right Top RightTop
enum	{ PERSPECTIVE=1, LEFT, RIGHT, FRONT, BACK, TOP, BOTTOM };	//orthoType

#define UPM 1.0f

class OgreRenderer;

class ANIMA_CORE_API View
{
public:
	View(int ortho, Ogre::Vector3 location, Ogre::Camera* cam, Ogre::Viewport* vp)
	{
		orthoType = ortho;
		center = Ogre::Vector3::ZERO;
		center[1] = 0.5*UPM;
		pos = location;
		wFull = false;
		hFull = false;
		ortho = false;

		camera = cam;
		viewPort = vp;
		orthoClipDistance = 100;
	}

	void init(OgreRenderer *renderer,Ogre::RenderWindow *renderWindow=NULL,bool haveSplit=true);
	void resize();

	int		id;			//GL id

	Ogre::Viewport*		viewPort;
	Ogre::Camera*		camera;
	//view window
	int		viewpos;	// L,R,T,B
	float	w,h;		// as 0-1
	float	x,y;		// as 0-1
	int		xp, yp;		//x,y in pixels
	int		wp, hp;		//w,h in pixels
	bool	wFull, hFull;	//indicates full size
	bool	ortho;
	int		orthoType;	//left, right, top etc
	//main
	void	renderGrid();
	void	rotateCameraOld(float pan, float tilt, const float zoom);
	void	rotateCamera(float pan, float tilt, const float zoom);
	void	panCamera(const float panx, const float pany, const float zoom);
	void	moveCamera();
	void	orbitCamera(const Ogre::Vector3 &O,float pan, float tilt, float zoom);
	void	zoomAll(const Ogre::Vector3& O,bool excludeBackground,bool onlySelected);

	void	set(const Ogre::Vector3 &pos,const Ogre::Vector3 &center);
	
	Ogre::Vector3	center;			//camera focus center (only use 2d for ortho)
	Ogre::Vector3	pos;
private:
	Ogre::RenderWindow *renderWindow;
	bool haveSplit;
	OgreRenderer *renderer;
	Ogre::Vector3	cameraRef;		//reference camera pos for use by certain camera functions
	float	orthoClipDistance;
};

#include "Anima_nsend.h"