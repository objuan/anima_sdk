#pragma once

#include "Core/SceneObjectGfx.h"
//#include <OgrePanelOverlayElement.h>
//#include <OgreOverlayElementFactory.h> 

#include "Anima_nsbegin.h"

class DynamicLines;
class SceneObjectMovable;
class SplineMovePath;
class SelectionManagerGfxSplineHelp;
class SelectionRectangle;
class SelectionManagerGfxMultiSplineHelp;
class MovePathManipolator;
class ANIMA_CORE_API SelectionManagerGfx : public SceneObjectGfx 
{
private:

	//SceneObject *currentObj;
	Ogre::SceneNode *selectionNode;
	SelectionRectangle *selectionMesh;

	Ogre::AxisAlignedBox selection_bbox;
	SceneObjectMovable *objMovable;
	Ogre::Vector3 moveOrigin;
	bool initialized;
	
protected:
	
	void OnPreRender(int group,int idx,SubSceneObjectGfx *gfxObj);
	bool moveUIMode;
	void SetMove(bool moveUIMode,SceneObjectMovable *obj);
	void OnChangeUI();

public :
	SelectionManagerGfx();
	~SelectionManagerGfx();

	void Init();
	/*void SetRotation(SceneObject *obj);
	void SetTranslation(SceneObject *obj);*/
	
	virtual AObject::Type GetType(){return AObject::AnimationType;}
	virtual QString ClassName(){return "SelectionManagerGfx";}
	virtual AObject::Type GetChildType(int idx){return AObject::AnimationType;}
	virtual int GetChildCount(AObject::Type type){return 0;}
	virtual SceneObject *GetChild(AObject::Type type,int idx){return NULL;}

	virtual DAG GetDAG(){return DAG("selection");}

	void OnSelectionBoxChanged();
	Ogre::AxisAlignedBox &GetSelectionBox(){return selection_bbox;}
	
	virtual QImage Image(){return ResourceManager::Singleton.item_group;}
	QColor GetColor(){return Qt::red;}
	QString GetName(){return "";}
	QString GetDesc(){return "";}
	virtual void Copy(SceneObject *fromObj){}
	virtual SceneObject *Clone(SceneObject *newParent){return NULL;}

	void Clear();
	void Remove(SceneObject *obj);

	bool IsMoveUIMode(){return moveUIMode; };
	Ogre::Vector3 GetMoveOrigin(){return moveOrigin;}
	void MoveAxes(Ogre::Vector3  &off);

	SelectionManagerGfxSplineHelp *GetSplineHelper();
	SelectionManagerGfxMultiSplineHelp *GetMultiSplineHelper();
	/*SelectionManagerGfxSplineHelp *NewSplineHelper(SplineMovePath *movePath);
	void DeleteSplineHelper(SelectionManagerGfxSplineHelp *movePath);*/

	void OnSelectionChanged();
	void OnEditModeChanged();
	virtual void OnPreRender();
};

class SelectionManagerEditStart: public SubSceneObjectGfx
{
public:

	SelectionManagerEditStart(SceneObjectGfx *obj);
	virtual ~SelectionManagerEditStart();

	virtual QString GetDesc(){return "SelectionManagerEditStart";}

};

class ANIMA_CORE_API SelectionRectangle : public Ogre::ManualObject
{
public:
    SelectionRectangle(const Ogre::String &name)
        : ManualObject(name)
    {
        setUseIdentityProjection(true);
        setUseIdentityView(true);
        setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
        setQueryFlags(0);
    }

    /**
     * Sets the corners of the SelectionRectangle.  Every parameter should be in the
     * range [0, 1] representing a percentage of the screen the SelectionRectangle
     * should take up.
     */
    void setCorners(float left, float top, float right, float bottom);

    void setCorners(const Ogre::Vector2 &topLeft, const Ogre::Vector2 &bottomRight)
    {
        setCorners(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
    }

};
class ANIMA_CORE_API SelectionRectangleOverlay : public Ogre::PanelOverlayElement
{
public:
    SelectionRectangleOverlay(const Ogre::String & name) : Ogre::PanelOverlayElement(name)
    {
    }
};
class ANIMA_CORE_API SelectionRectangleOverlayFactory : public Ogre::OverlayElementFactory
{
public:
    static Ogre::String typeName;
    virtual Ogre::OverlayElement *  createOverlayElement (const Ogre::String &instanceName)
    {
        return New SelectionRectangleOverlay(instanceName);
    }
    virtual void  destroyOverlayElement (Ogre::OverlayElement *pElement)
    {
        delete( pElement);
    }
    virtual const Ogre::String & getTypeName (void) const {return typeName;}
};
#include "Anima_nsend.h"