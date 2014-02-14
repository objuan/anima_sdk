#pragma once

#include "Anima_nsbegin.h"
class FilmLayer;
class ProgressBar;
class Animation;
class AnimationPart;
class GfxContextScreenGrab;

class ANIMA_CORE_API AnimaFileInfo
{
public:
	QString Path;
	bool HaveCollision;
	int NumLayers;
	int NumAnim;
	int NumActors;
	int NumClones;
	int NumFaces;
	int NumVertices;
	QPixmap *tumbNail;

	AnimaFileInfo():NumLayers(0) ,NumAnim(0),NumActors(0),NumClones(0),NumFaces(0),NumVertices(0){tumbNail=NULL;}
	~AnimaFileInfo(void) {if (tumbNail!=NULL) delete tumbNail;}
};

class ANIMA_CORE_API ContextIO: public BObject
{
private:
	QString importMessage;
	int maxModelID;
	static QString loadingPath;
	static QString savingPath;
public:
	ContextIO(void);
	~ContextIO(void);

	int animPartIdx;
	QList<Ogre::Vector3> lastPathPoints;

	static QString GetSavingPath(){return savingPath;}
	static QString GetLoadingPath(){return loadingPath;}

	void LoadLayer(FilmLayer *layer,QDomElement &filmEle,bool fromMerge=false);

	AnimaFileInfo *PreloadFileInfo(const QString &filePath);

	bool Load(const QString &filePath,bool onlyAnim,ProgressBar *progressBar);
	bool Load(QDomDocument &doc,const QString &filePath,bool onlyAnim,ProgressBar *progressBar);
	bool Load(const QString &aniData,const QString &filePath,bool onlyAnim,ProgressBar *progressBar);

	bool Save(const QString &filePath,bool saveCollision,ProgressBar *progressBar,GfxContextScreenGrab *tumbNail);
	bool Save(QTextStream &out,const QString &fileName,bool saveCollision,ProgressBar *progressBar,GfxContextScreenGrab *tumbNail);
	QString GetSave(const QString &fileName,bool saveCollision,ProgressBar *progressBar);

	QString &GetImportMessage(){return importMessage;}

	void LoadAnimation(Animation *anim,QDomElement &animEle,bool fromMerge);
	void SaveAnimation(Animation *anim,QDomDocument &doc,QDomElement &ele,bool onlyPath,bool saveCollision  );
	void LoadAnimationPart(AnimationPart *part,QDomElement &animPartEle,bool fullInfo);
	AnimationPart *LoadAnimationPart(Animation *anim,QDomElement &animPartEle,int pos=-1);
	void SaveAnimationPart(Animation *anim,AnimationPart *animPart,QDomDocument &doc,QDomElement &ele,bool fullInfo );

	static QDomElement GetChild(QDomElement &ele,int idx);
	static QColor ReadColor(QDomElement &ele);
	static void WriteColor(QDomElement &ele,QColor &color);
};
#include "Anima_nsend.h"