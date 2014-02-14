#pragma once

#include "Core/Mesh.h"
#include "Anima_nsbegin.h"

class ANIMA_CORE_API ImportGeometry: public BObject
{
public:

	enum ImportGeometryType
	{
		Static,
		Dynamic
	};
	// struttura:
	//   rootNode
	//		-  mesh: layer 0
	//			.........
	//		-  mesh: layer n
	//
	IMesh *Load(const QString &filePath,const QString &meshName,IMesh::CollisionType collisionType,ImportGeometryType type,const Ogre::Vector3 &rot,const QString &mat);
};
#include "Anima_nsend.h"