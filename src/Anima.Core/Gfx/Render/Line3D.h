#pragma once
#ifndef __LINE3D_H__
#define __LINE3D_H__

using namespace std;

#include "Anima_nsbegin.h"

class Line3D:public Ogre::SimpleRenderable
{
public:
   Line3D(void);
   ~Line3D(void);

   void addPoint(const Ogre::Vector3 &p);
   const Ogre::Vector3 &getPoint(unsigned short index) const;
   unsigned short getNumPoints(void) const;
   void updatePoint(unsigned short index, const Ogre::Vector3 &value);
   void drawLine(Ogre::Vector3 &start, Ogre::Vector3 &end);
   void drawLines(void);

   Ogre::Real getSquaredViewDepth(const Ogre::Camera *cam) const;
   Ogre::Real getBoundingRadius(void) const;
protected:
   //void getWorldTransforms(Matrix4 *xform) const;
   const Ogre::Quaternion &getWorldOrientation(void) const;
   const Ogre::Vector3 &getWorldPosition(void) const;

   vector<Ogre::Vector3> mPoints;
   bool mDrawn;
};

#include "Anima_nsend.h"

#endif /* __LINE3D_H__ */
