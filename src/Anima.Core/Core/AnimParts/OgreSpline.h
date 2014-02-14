#pragma once

#include "Anima_nsbegin.h"

#ifndef __OgreSpline_H__
#define __OgreSpline_H__

//#include "OgrePrerequisites.h"
//#include "OgreOgre::Vector3.h"
//#include "OgreMatrix4.h"

/** A very simple spline class which implements the Catmull-Rom class of splines.
@remarks
    Splines are bendy lines. You define a series of points, and the spline forms
    a smoother line between the points to eliminate the sharp angles.
@par
    Catmull-Rom splines are a specialisation of the general Hermite spline. With
    a Hermite spline, you define the start and end point of the line, and 2 tangents,
    one at the start of the line and one at the end. The Catmull-Rom spline simplifies
    this by just asking you to define a series of points, and the tangents are 
    created for you. 
*/
class ANIMA_CORE_API OgreSpline: public BObject
{
public:
    OgreSpline();
    ~OgreSpline();

    /** Adds a control point to the end of the spline. */
    void addPoint(const Ogre::Vector3& p);

	void deletePoint(int idx);

    /** Gets the detail of one of the control points of the spline. */
    const Ogre::Vector3& getPoint(unsigned short index) const;

    /** Gets the number of control points in the spline. */
    unsigned short getNumPoints(void) const;

    /** Clears all the points in the spline. */
    void clear(void);

    /** Updates a single point in the spline. 
    @remarks
        This point must already exist in the spline.
    */
    void updatePoint(unsigned short index, const Ogre::Vector3& value);

    /** Returns an interpolated point based on a parametric value over the whole series.
    @remarks
        Given a t value between 0 and 1 representing the parametric distance along the
        whole length of the spline, this method returns an interpolated point.
    @param t Parametric value.
    */
    Ogre::Vector3 interpolate(Ogre::Real t);

    /** Interpolates a single segment of the spline given a parametric value.
    @param fromIndex The point index to treat as t=0. fromIndex + 1 is deemed to be t=1
    @param t Parametric value
    */
    Ogre::Vector3 interpolate(unsigned int fromIndex, Ogre::Real t);

	Ogre::Vector3 interpolateFixSpace(Ogre::Real t);

	void startInterpolate(unsigned int segmentIdx);
	void interpolate(unsigned int segmentIdx,double t,Ogre::Vector3 &p);

    /** Tells the spline whether it should automatically calculate tangents on demand
        as points are added.
    @remarks
        The spline calculates tangents at each point automatically based on the input points.
        Normally it does this every time a point changes. However, if you have a lot of points
        to add in one go, you probably don't want to incur this overhead and would prefer to 
        defer the calculation until you are finished setting all the points. You can do this
        by calling this method with a parameter of 'false'. Just remember to manually call 
        the recalcTangents method when you are done.
    @param autoCalc If true, tangents are calculated for you whenever a point changes. If false, 
        you must call reclacTangents to recalculate them when it best suits.
    */
    void setAutoCalculate(bool autoCalc);

    /** Recalculates the tangents associated with this spline. 
    @remarks
        If you tell the spline not to update on demand by calling setAutoCalculate(false)
        then you must call this after completing your updates to the spline points.
    */
    void recalcTangents(void);

	void setTangent(int idx,const Ogre::Vector3 &t)
	{
		user_mTangents[idx] = t;
		if (mAutoCalc)
		{
			recalcTangents();
		}
	}

	OgreSpline *getEqualDistanceSpline(Ogre::Real wantedDistance);
	OgreSpline *getEqualDistanceSpline1(Ogre::Real wantedDistance);
	OgreSpline *getEqualDistanceSpline2(Ogre::Real wantedDistance);
	Ogre::Real getClosestPointOnSpline(Ogre::Vector3 testPoint, Ogre::Real s1, Ogre::Real s2, Ogre::Real s3, int maxIterations = 20);

protected:

    bool mAutoCalc;

    std::vector<Ogre::Vector3> mPoints;
    std::vector<Ogre::Vector3> mTangents;
	std::vector<Ogre::Vector3> user_mTangents;

    /// Matrix of coefficients 
	Ogre::Matrix4 mCoeffs;


	Ogre::Matrix4 pt;

};


#include "Anima_nsend.h"
#endif
