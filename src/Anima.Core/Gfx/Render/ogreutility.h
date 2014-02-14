#pragma once

#include "Anima_nsbegin.h"

#ifndef PI
#define PI	(3.141592654f)
#endif

#ifndef TWOPI
#define TWOPI (6.283185307f)
#endif

// degrees to radians multiplier
// eg radians = degrees * DEG2RAD;
#ifndef DEG2RAD
#define DEG2RAD (0.017453292f)
#endif

// degrees = radians * RAD2DEG;
#ifndef RAD2DEG
#define RAD2DEG	(57.29577951f)
#endif

#ifndef SQRT_OF_2
#define SQRT_OF_2 (1.4142135f)
#endif


//wchar_t* convertToWideChar(const char* p);
//LPTSTR copyString(std::string src);/
//const LPTSTR constCopyString(std::string src);
//char* strWcs(LPTSTR src);
////
////inline Ogre::Vector3 toOgre(const Ogre::Vector3 &v) {
////	return Ogre::Vector3(v.x, v.y, v.z);
////}
////inline Ogre::Quaternion toOgre(const Ogre::Quaternion &q) {
////    return Ogre::Quaternion(q.w, q.x, q.y, q.z);
////}
////inline Ogre::Vector3 toNx(const Ogre::Vector3 &v) {
////    return Ogre::Vector3(v.x, v.y, v.z);
////}
////inline Ogre::Matrix3 &toNx(Ogre::Matrix3 &m) {
////    return *(Ogre::Matrix3*)&m;
////}
////
////inline Ogre::Quaternion toNx(Ogre::Quaternion &q) {
////	Ogre::Matrix3 m;
////	q.ToRotationMatrix(m);
////	Ogre::Matrix3 nm = toNx(m);
////	return Ogre::Quaternion(nm);
////}
////
////inline Ogre::Matrix3 & toOgre(Ogre::Matrix3 &m) {
////    return *(Ogre::Matrix3*)&m;
////}

Ogre::Quaternion nxMatToNode(Ogre::Matrix3 m33);
void nodeToAxis(const Ogre::SceneNode* n, Ogre::Vector3 &x, Ogre::Vector3 &y, Ogre::Vector3 &z);
Ogre::Matrix3 nodeToMat(const Ogre::SceneNode* n);
Ogre::Matrix3 quatToMat(const Ogre::Quaternion n);
Ogre::Matrix3 nodeToOgreMat(const Ogre::SceneNode* n);
Ogre::Quaternion nxMatToNode(Ogre::Vector3 a, Ogre::Vector3 b, Ogre::Vector3 c);
void flipZtoY(Ogre::SceneNode* node);
Ogre::Vector3 getNodeColumn(Ogre::SceneNode* node, int i);
float getAngle(const Ogre::Vector3 axis, const Ogre::Vector3 v1o, const Ogre::Vector3 v2o);
void ZConvert( Ogre::Vector3 &v1, Ogre::Vector3 &v2, Ogre::Vector3 &v3 , Ogre::Vector3 &pos , bool loading);
void matToAxis( const Ogre::Matrix3 m, Ogre::Vector3 &v0, Ogre::Vector3 &v1, Ogre::Vector3 &v2);

inline Ogre::Quaternion scyVecToQuat(const Ogre::Vector3& v1, const Ogre::Vector3& v2, const Ogre::Vector3& v3)
{
	Ogre::Matrix3 m;
	m.SetColumn(0, v1);
	m.SetColumn(1, v2);
	m.SetColumn(2, v3);

	Ogre::Quaternion q;
	q.FromRotationMatrix(m);
	return q;
}

void rotateAxis(Ogre::Vector3 &v, float x, float y, float z, float angle);
float getAngle(Ogre::Vector3 axis, Ogre::Vector3 v1o, Ogre::Vector3 v2o);
void matNxToVec(const Ogre::Matrix3 m, Ogre::Vector3 &v1, Ogre::Vector3 &v2, Ogre::Vector3 &v3);
void mat33Rotate(float angle, float x, float y, float z, Ogre::Matrix3 &nm);
void nxVecToMat(const Ogre::Vector3 v1, const Ogre::Vector3 v2, const Ogre::Vector3 v3, Ogre::Matrix3 &m);
void mat33RotateGlobal(float angle, float x, float y, float z, Ogre::Matrix3 &m);
void quatToNxMat(const Ogre::Quaternion q, Ogre::Matrix3 &m);
void NxMatToQuat(Ogre::Quaternion &q, const Ogre::Matrix3 m);


#include "Anima_nsend.h"
