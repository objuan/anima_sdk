//--------------------------------------------------------------
//ANIMA project
//File: SplineDesignMode.h
//Description: mode for designing spline path
//Author: Voloshin Ivan
//--------------------------------------------------------------
#pragma once
#include "Core/Action/ActionModes/PathDesignMode.h"
class SplineMovePath;
#include "Anima_nsbegin.h"
class SplineDesignMode : public PathDesignMode
{
public:
    SplineDesignMode();
    ~SplineDesignMode();

    virtual Editor::PathModeType getPathType();
    virtual bool activate();
    virtual bool deactivate();
    virtual bool isActive();
    virtual MovePath* getPath();
    virtual Animation * getAnimation();
private:
    bool b_active;
    SplineMovePath * m_path;
    Animation * m_animation;
};
#include "Anima_nsend.h"
