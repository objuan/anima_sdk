#pragma once
#ifndef __TextureRenderer_H__
#define __TextureRenderer_H__

#include "Anima_nsbegin.h"

//class TxtFrameListener : public ExampleFrameListener 
//{ 
//protected: 
//	Ogre::Camera* mReflectCam; 
//    Ogre::SceneNode* mPlaneNode; 
//public: 
//    TxtFrameListener(RenderWindow* window, Camera* maincam, Camera* reflectCam,SceneNode* planeSceneNode) 
//        :ExampleFrameListener(window, maincam),mReflectCam(reflectCam), mPlaneNode(planeSceneNode){} 
//    bool TxtFrameListener(const FrameEvent& evt) 
//    { 
//        if( ExampleFrameListener::frameStarted(evt) == false )return false; 
//        // Make sure reflection camera is updated too 
//        mReflectCam->setPosition(mCamera->getPosition()); 
//        mReflectCam->setOrientation(mCamera->getOrientation()); 
//        mPlaneNode->yaw(Degree(30 * evt.timeSinceLastFrame), Node::TS_PARENT); 
//        return true; 
//    } 
//}; 

class TextureRenderer 
{ 
	void Add();
};

#include "Anima_nsend.h"

#endif
