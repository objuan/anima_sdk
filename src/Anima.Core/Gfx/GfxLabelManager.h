#pragma once
#include "Anima_nsbegin.h"
//class GfxModel;
class SubSceneObjectGfx;
class GfxLabel;

class ANIMA_CORE_API GfxLabelManager: public BObject
{
private:

	Ogre::OverlayManager*    _overlayMgr;

	Ogre::Overlay*          overlayBadCurveAngles;
	Ogre::Overlay*			overlayBadCurvePointMax;
	Ogre::Overlay*			overlayBadCurvePointMin;
	Ogre::Overlay*			overlayBadCurveAll;
	Ogre::Overlay*			overlayBadClipSize;
	Ogre::Overlay*			overlayBadStair;
	Ogre::Overlay*			overlayBadPathPointNear;

	int _ID;
	Ogre::SceneNode *rootNode;

	Ogre::BillboardSet *set;

	QList<GfxLabel *> labelList;
	GfxLabel *tooltipLabel;
	Ogre::Overlay* tooltipOverlay;
	bool init;
	int w,h;

private:
	
	int Find(SubSceneObjectGfx *obj);
	void GetPosition(SubSceneObjectGfx *obj,int *x,int *y);

public:

	GfxLabelManager();
	virtual ~GfxLabelManager();

	void Init();

	void OnRemove(SubSceneObjectGfx *obj);

	GfxLabel *AddLabel(SubSceneObjectGfx *obj,const QString &text,const Ogre::ColourValue& color = Ogre::ColourValue(1.0, 1.0, 1.0, 1.0),const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);
	GfxLabel *AddLabel(SubSceneObjectGfx *obj,const QString &text,float width, float height,const Ogre::ColourValue& color = Ogre::ColourValue(1.0, 1.0, 1.0, 1.0),const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);
	void DelLabel(SubSceneObjectGfx *obj);
	void ChangeLabel(SubSceneObjectGfx *obj,const QString &text,const Ogre::ColourValue& color,const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);

	void InvalidateGfx();
	void RebuildBounds();
	void Update();

	// x,y tra 0 e 1
	GfxLabel *GetLabel(int mx,int my,int w,int h);

	void OnToolTipOpen(GfxLabel *label);
	void OnToolTipClose();
};

class ANIMA_CORE_API GfxLabel: public BObject
{
public:
	Ogre::String ID;
	QString label;
	SubSceneObjectGfx *obj;
	Ogre::SceneNode *node;
	Ogre::Billboard *bil ;
	Ogre::Vector3 pos;

	double scale;
	double bound_x[2];
	double bound_y[2];
	double z;
};

#include "Anima_nsend.h"