#pragma once

#include <QString>
#include <fbxsdk.h>
#include <fbxfilesdk_nsuse.h>

class GfxModel;
class Clip;

class GfxMorphBuilder
{
public:
	GfxMorphBuilder(void);
	~GfxMorphBuilder(void);

	void BuildMorphTake(GfxModel *model,QString &name);
	void SetMorphTake(GfxModel *model,QString &name,Clip *from,KTime fromRelativeEndTime,Clip *to,KTime morphTime);

};
