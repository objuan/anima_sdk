#pragma once
#ifndef __DEBUGGRAPHICS_H__
#define __DEBUGGRAPHICS_H__

using namespace std;

#include "Anima_nsbegin.h"

class DebugGraphics : public Ogre::FrameListener {

public:
    DebugGraphics(Ogre::SceneManager *sceneMgr, int maxVertices = 65536);
    ~DebugGraphics();

    void addPoint(const Ogre::Vector3 &p);
    void addLine(const Ogre::Vector3 &a, const Ogre::Vector3 &b);
    void addTriangle(const Ogre::Vector3 &a, const Ogre::Vector3 &b, const Ogre::Vector3 &c);
    void setColour(const Ogre::ColourValue &c);

    bool frameStarted(const Ogre::FrameEvent &evt);

    class PImpl;

private:
    PImpl *p;
};

#include "Anima_nsend.h"

#endif
