#pragma once

#include "./ResourceManager.h"

#include "Anima_nsbegin.h"


class ANIMA_CORE_API SceneObjectMaterial 
{
private:

	SceneObjectMaterial *parentMaterial;
	bool haveColor;
	QColor color;
	QString materialName;
	SceneObject *sceneObject;

	Ogre::MaterialPtr mat;
//	Ogre::MaterialPtr mat_selected;

private:

	void UpdateMaterial();
	void ApplyEffects(Ogre::MaterialPtr mat);

public:

	SceneObjectMaterial(SceneObjectMaterial *parentMaterial,SceneObject *so,const QString &baseName);
	virtual ~SceneObjectMaterial();

	QString GetMaterialName();

	QColor GetColor();
    void SetColor(const QColor &col);

	//Ogre::MaterialPtr createPathMaterial(const QString &name, const QString & group, const Ogre::Vector4 &color);


	//static void ApplyEffect(Ogre::Entity *entity,bool selected,bool over);

};
#include "Anima_nsend.h"

