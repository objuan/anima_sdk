//--------------------------------------------------------------
//ANIMA project
//File: PathDesignMode.h
//Description: base class for all path designing modes
//Author: Voloshin Ivan
//--------------------------------------------------------------
#pragma once

#include "Core/Edit/EditorManager.h"
#include "Core/Animation.h"
#include "Core/MovePath.h"

#include "Anima_nsbegin.h"
class PathDesignMode: QObject
{
    Q_OBJECT
public:
    virtual Editor::PathModeType getPathType() = 0;
    
    //activate path designing mode, returns true if successfully activated
    virtual bool activate() = 0;
    
    //deactivate path designing mode,
    //returns false when not deactivated (i.e. user didn't confirmed deactivation)
    virtual bool deactivate() = 0;

    //returns true if mode is active
    virtual bool isActive() = 0;

    //returns currently designing path
    virtual MovePath* getPath() = 0;
    //returns path's parent animation
    virtual Animation * getAnimation() = 0;

signals:
    void beforeActivate();
    void afterActivate();
    void beforeDeactivate();
    void afterDeactivate();
};
#include "Anima_nsend.h"