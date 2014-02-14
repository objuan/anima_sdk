#pragma once

#include "SceneObject.h"
#include "SceneLibrary.h"
#include "SceneObjectMaterial.h"

#include "Anima_nsbegin.h"
class Film;
class Animation;
class IMesh;
class Model;
class BaseModel;
class ModelGroup;

class ANIMA_CORE_API FilmLayer : public SceneObjectMovable , public SceneObjectMaterial , public SceneObjectUI, public SceneBlockElement
{
friend class Film;
friend class AnimationPartAddCommand;
friend class RenameCommand;

private:
    Film *film;
	QString Name;
    QList<QColor> animationsStandartColors;
    int nextNewAnimationColor;

	QList<Animation *> animationList;
	//QColor Color;
   // QString materialName;
protected:
	virtual void ComputeBound();
	virtual void OnEnsureTrx();

public:
	FilmLayer(Film *film);
	~FilmLayer(void);
    
	double GetMinTimeBound();
	double GetMaxTimeBound();

    QColor GetNextNewAnimationColor();

	virtual AObject::Type GetType(){return AObject::FilmLayerType;}
	virtual QString ClassName(){return "FilmLayer";}
	//virtual SceneObject *GetParent(){return NULL;}
	virtual AObject::Type GetChildType(int idx){return AObject::AnimationType;}
	virtual int GetChildCount(AObject::Type type){return animationList.count();}
	virtual SceneObject *GetChild(AObject::Type type,int idx);

	virtual DAG GetDAG(){return DAG(Name);}
	virtual void Copy(SceneObject *fromObj);
	virtual SceneObject *Clone(SceneObject *newParent);

	virtual QImage Image(){return ResourceManager::Singleton.item_group;}
	//QColor GetColor(){return Color;}
   // void SetColor(const QColor & col) {Color = col; UpdateMaterial();}
   // void UpdateMaterial();
   // QString GetMaterialName() {return materialName;}
	QString GetName(){return Name;}
    void SetName(const QString &name);
	QString GetDesc(){return "";}

	QList<Animation *> &GetAnimationList(){return animationList;}
	Film *GetFilm(){return film;}
	int AnimationIdx(Animation *animation);
	Animation *AddAnimation();
	Animation *AddAnimation(Animation *anim);
	void InsertAnimation(int idx,Animation *anim);
	void DeleteAnimation(Animation *animation);
	int IndexOf(Animation *filmLayer);
	Animation *Find(const QString &name);
	bool RenameAnimation(Animation *animation,const QString &newName);
	bool Contains(Animation *anim);

	// info
	double StartFrame();
	double EndFrame();
	double FrameCount();

	void Clear();
	void OnTimerTick();
	void OnFPSChanged();
	void OnLoad();

	virtual void InvalidateBuild(bool pathChanged);
	//virtual void InvalidateGfx();
	void InvalidateGfxFromIK(BaseModel *model);

	// IO
	void Save(QDomDocument &doc,QDomElement &ele );
	static FilmLayer *Load(QDomElement &ele );

	QString ToString();

};
#include "Anima_nsend.h"
