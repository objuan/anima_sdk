#pragma once

//#include "SDK/Texture.h"
#include "Anima_nsbegin.h"

class GfxScene;
class GfxModel;
class GfxClip;
class ClipNode;

class ANIMA_CORE_API GfxImport
{
public:
	int idx;
	GfxImport(void);
	~GfxImport(void);

private:
	//void FillAnimData(KFbxNode *node,QList<ClipNode *> &clipNodes,QString &originalName,QString &name);

public:
	void LoadModel(GfxScene *gfxScene,GfxModel *gfxModel);
	void LoadClip(GfxScene *gfxScene,GfxClip *clip);
//	void ClearAnim(KFbxNode *node,const QString &animName);

private:

//	void GetModelInfo(GfxModel *gfxModel,KFbxNode* pNode,int liv=0);
};
#include "Anima_nsend.h"