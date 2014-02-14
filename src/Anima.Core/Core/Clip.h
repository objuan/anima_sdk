#pragma once

#include "Item.h"

#include "Anima_nsbegin.h"
enum ClipType
{
	ClipType_Lib =0,
	ClipType_Link = 2,
};

enum Clip_ResizeType
{
	Clip_NoResize = 0,
	Clip_DistanceResize,
	Clip_HeightResize,
	Clip_HeightLegsResize,
	Clip_AllResize
};

enum Clip_ComposeType
{
	Clip_Compose_KeepOriginal = 0,
	Clip_Compose_LastResize,
	Clip_Compose_FollowPath,
};

class AnimClipModelFunction;
class MeshEditable;
class AnimClipModel;
class ModelClip;
class MovePath;
class QRandom;

class ANIMA_CORE_API Clip : public BObject 
{
	friend class ClipLibrary;
private:
	bool isVirtual;
	bool preProcess;

	Clip_ResizeType resizeType;
	Clip_ComposeType composeType;
	double pathOffset;
	AnimClipModelFunction *condition;
	QList<Clip *> subClipList;
	Clip *parent;
	Clip *subDefaultClip;
	Clip *lastClip;
	QString meshName;
	QString base_meshName;
	MeshEditable *mesh;
    QColor Color;
	bool noOverlayMode;

public:

	QString ID;
	QString Name;
	QString Desc;
//	QString Category;
	//QString Material;
	AnimatedImage image;

	Clip(Clip *parent);
	virtual ~Clip();

	Clip *GetParentClip(){return parent;}

	void LoadRule(QDomElement e);
	void LoadRuleEnd(QDomElement e);
	virtual void Load(QDomElement e);

	bool IsVirtual(){return isVirtual;}
	bool NeedPreProcess(){return preProcess;}
	bool NoOverlayMode(){return noOverlayMode;}

	virtual bool IsDynamic()=0;
	virtual bool IsPose()=0;
	virtual bool IsFixed()=0;
	virtual ClipType Type()=0;
	virtual bool NeedCopyForBlend(){return false;}

	QString &GetName(){return Name;}
	QString &GetDesc(){return Desc;}
	QColor &GetColor(){return Color;}
    void SetColor(const QColor &color) {Color = color;}
//	QString &GetMaterial(){return Material;}
//	QString &GetCategory(){return Category;}
	AnimatedImage &GetImage(){return image;}

	//SelectedActiveFootType GetStartFootType(){return startFootType;}
//	SelectedActiveFootSideType GetStartFootSideType(){return startFootSideType;}
	QList<Clip *> &GetSubClipList(){return subClipList;}
	AnimClipModelFunction *GetConditionFun(){return condition; };
	Clip_ResizeType GetResizeType(){return resizeType;}
	Clip_ComposeType GetComposeType(){return composeType;}
	Clip *GetLastClip(){return lastClip;}
	double GetPathOffset(){return pathOffset;}
	QString GetMeshName();
	QString GetBaseMeshName();
};

// ===============================================================

class AnimClip;
class LinkClip;
class ModelExporterParams;
typedef QList<LinkClip *> LinkClipList;

class LinkClipTargetInfo: public BObject 
{
public:
	bool isLeftSide;
	int sideCount; // == -1 no, prima di sideCount sono a sinistra, dopo a destra
 
	QString condition;
	LinkClipList clipList;
//	bool linked;

	LinkClipTargetInfo(){}

	bool IsValid(int idx,bool isLeftSide);
};


class LinkClipInfo: public BObject 
{
public:
	QList<LinkClipTargetInfo *> TargetList;

	LinkClipInfo();
	~LinkClipInfo();
};

enum LibClip_BuildMode
{
	Auto = 0,
	UserSpline = 1
};

class ANIMA_CORE_API LibClip : public Clip 
{
	friend class AnimClip;
private:
	QHash<QString,LinkClipInfo *> targetMap;
public:
	bool isDynamic;
	bool isPose;
	bool isFixed;
	bool noloop;						
	double defaultSize;
	double minimumSize;
	bool needCopyForBlend;

	// build path
	int post_build_mode;
	int pre_build_mode;

	QString exportRelativePath;
	ModelExporterParams *exportParams;

public:
	LibClip(Clip *parent) ;
	virtual ~LibClip();

	virtual void Load(QDomElement e);

	virtual bool IsDynamic(){return isDynamic;}
	virtual bool IsPose(){return isPose;}
	virtual bool IsFixed(){return isFixed;}
	virtual bool NeedCopyForBlend(){return needCopyForBlend;}
	bool NoLoop(){return noloop;}
	double GetDefaultSize(){return defaultSize;}
	double GetMinimumSize(){return minimumSize;}
	void AddTargetSequence(LibClip *target,QStringList &seqIDList,const QString &side,QString &condition,bool linked);
	bool GetClipSequence(LibClip *target,QList<LinkClip *> &linkList,bool sideFromPrev);

	virtual ClipType Type(){return ClipType_Lib;}

	bool IsBuildMode_Pre(LibClip_BuildMode mode);
	bool IsBuildMode_Post(LibClip_BuildMode mode);

	ModelExporterParams *GetExportParams();
};

class ANIMA_CORE_API LinkClip : public Clip 
{
public:
	Clip *From;
	Clip *To;
public:
	
	LinkClip(Clip *parent) : Clip(parent){}
	virtual ~LinkClip();

	virtual void Load(QDomElement e);

	virtual ClipType Type(){return ClipType_Link;}
	virtual bool IsDynamic(){return false;}
	virtual bool IsPose(){return false;}
	virtual bool IsFixed();
};

enum OverlayClip_ComposeMode
{
	OverlayClip_ComposeMode_ADD =0,
	OverlayClip_ComposeMode_REPLACE
};

class ANIMA_CORE_API OverlayClip : public BObject 
{
private:

	QString id;
	QString name;
	QColor color;
	QImage image;
	double weight;
	OverlayClip_ComposeMode composeMode;

	QList<QString> targetClipList;
	QList<QString> targetPathList;

	// GFX
public:

	QString bone;
	QStringList boneExclude;

	double beginTime;
	double endTime;

	double clipTime; // letto dall'animazione

public:
	
	OverlayClip(){}

	QString &ID(){return id;}
	QString &GetName(){return name;}
	QColor &GetColor(){return color;}
	QImage &GetImage(){return image;}

	double GetWeight(){return weight;}
	OverlayClip_ComposeMode GetComposeMode(){return composeMode;}

	virtual void Load(QDomElement e);
	
	bool CanTarget(ModelClip *modelClip,MovePath *path);
	
};

class ANIMA_CORE_API OverlayGroupClip : public BObject 
{
private:

	QString id;
	QString name;
	bool isDefault;
	QList<OverlayClip *> clipList;

public:
//	OverlayGroupClip();

	QString &ID(){return id;}
	QString &GetName(){return name;}
	bool IsDefault(){return isDefault;}
	virtual void Load(QDomElement e);

	OverlayClip *Assign(ModelClip *modelClip,MovePath *path,QRandom &rnd);
};

#include "Anima_nsend.h"