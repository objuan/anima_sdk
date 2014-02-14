#pragma once

#include "Context.h"
#include "Item.h"

#include "Anima_nsbegin.h"
class Clip;
class LibClip;
class LinkClip;
class OverlayClip;
class OverlayGroupClip;
class QRandom;

class ANIMA_CORE_API ClipLibrary   : public BObject
{
friend class Clip;
private:

	QList<Clip *> allClipList;
	QList<Clip *> clipList;
	QHash<QString,Clip *> clipMapID;
	QHash<QString,Clip *> clipMapName;

	QHash<QString,OverlayClip *> clipOverlayMapID;
	QList<OverlayClip *> overlayClipList;

	QHash<QString,OverlayGroupClip *> clipOverlayGroupMapID;
	QList<OverlayGroupClip *> overlayGroupClipList;
	QList<OverlayGroupClip *> overlayDefaultGroupClipList;

	QString debugCollisionClip;
	//QList<QString> categoryList;

private:

	Clip *LoadSubClip(Clip *parentClip,QDomElement e);
	void LoadClips(QDomElement e);
	void LoadLinks(QDomElement e,bool loadClipSequence);
	void LoadOverlayClip(QDomElement e);
	void LoadOverlayGroupClip(QDomElement e);

public:
	ClipLibrary(void);
	~ClipLibrary(void);

	QList<Clip *> &AllClipList(){return allClipList;}
	QList<Clip *> &ClipList(){return clipList;}
	QList<OverlayClip *> &OverlayClipList(){return overlayClipList;}
	QList<OverlayGroupClip *> &OverlayGroupClipList(){return overlayGroupClipList;}

	QString GetDebugCollisionClip(){return debugCollisionClip;}

	void Load();
	void ReloadBrain();

	Clip *GetStairClip(bool directionUp);
	Clip *GetEscalatorClip(bool directionUp);
	Clip *FindByID(const QString &id);
	Clip *Find(const QString &name);

	OverlayClip *_FindOverlayByID(const QString &id);
	OverlayGroupClip *_FindOverlayGroupByID(const QString &id);
	//OverlayGroupClip *_GetDefaultOverlayGroup(QRandom &rnd);

};
#include "Anima_nsend.h"

