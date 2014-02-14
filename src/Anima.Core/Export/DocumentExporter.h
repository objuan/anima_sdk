#ifndef _DOCUMENT_EXPORTER_H_
#define _DOCUMENT_EXPORTER_H_

class FCDocument;
class ColladaOptions;
class AnimationExporter;
class CameraExporter;
class ControllerExporter;
class EntityExporter;
class GeometryExporter;
class LightExporter;
class NodeExporter;
class MaterialExporter;
class ForceExporter;
class EmitterExporter;
class XRefManager;
class FCDSceneNode;

void copyString(wchar_t *dst,char *src);
void copyString(wchar_t *dst,const char *src);

#define ANIM document->GetAnimationExporter()
#define CDOC document->GetCOLLADADocument()
#define ENTE document->GetEntityExporter()
#define OPTS document->GetColladaOptions()
#define NDEX document->GetNodeExporter()
#define MATE document->GetMaterialExporter()

class BoneInfo
{
public:

	Ogre::Bone *bone;
	FCDSceneNode* colladaNode;

	BoneInfo(Ogre::Bone *bone,FCDSceneNode* colladaNode) : bone(bone) , colladaNode(colladaNode)
	{
	}
};

// Main exporter class
class DocumentExporter
{
private:
//	Interface*		pMaxInterface;		// The main MAX UI interface
	ColladaOptions* options;			// The options container
	FCDocument*		colladaDocument;	// The FCollada document to fill in and write out.

	QList<BoneInfo> boneList;
	// Sub-library exporter(s)
	AnimationExporter* animationExporter;
	CameraExporter* cameraExporter;
	ControllerExporter* controllerExporter;
	EntityExporter* entityExporter;
	GeometryExporter* geometryExporter;
	LightExporter* lightExporter;
	MaterialExporter* materialExporter;
	NodeExporter* nodeExporter;
	XRefManager* xRefManager;

private:

	void ExportInstances();
	void ExportSkeleton(FCDSceneNode* parent, Ogre::Bone *bone);

public:
	DocumentExporter(ColladaOptions *options);
	~DocumentExporter();

	// utils

//	FCDSceneNode* FindExportedNode(Ogre::mesh* node);

	int GetBoneCount(){return boneList.count(); }
	int NodeIndex(Ogre::Bone *bone);
	BoneInfo GetBone(int idx){return boneList[idx];}

	// Accessors
	inline FCDocument* GetCOLLADADocument() { return colladaDocument; }
//	inline Interface* GetMaxInterface() { return pMaxInterface; }
	inline ColladaOptions* GetColladaOptions() { return options; }

	inline AnimationExporter* GetAnimationExporter() { return animationExporter; }
	inline CameraExporter* GetCameraExporter() { return cameraExporter; }
	inline ControllerExporter* GetControllerExporter() { return controllerExporter; }
	inline EntityExporter* GetEntityExporter() { return entityExporter; }
	inline GeometryExporter* GetGeometryExporter() { return geometryExporter; }
	inline LightExporter* GetLightExporter() { return lightExporter; }
	inline MaterialExporter* GetMaterialExporter() { return materialExporter; }
	inline NodeExporter* GetNodeExporter() { return nodeExporter; }
	inline XRefManager* GetXRefManager() { return xRefManager; }
	// Export
	void ExportCurrentMaxScene();
	void ExportAsset();
	bool ShowExportOptions(BOOL suppressOptions);
};


//inline Matrix3 ToMatrix3(const Ogre::Matrix4& mx)
//{
//	Point3 u(mx[0][0], mx[0][1], mx[0][2]);
//	Point3 v(mx[1][0], mx[1][1], mx[1][2]);
//	Point3 n(mx[2][0], mx[2][1], mx[2][2]);
//	Point3 t(mx[3][0], mx[3][1], mx[3][2]);
//	return Matrix3(u, v, n, t);
//}

inline FMMatrix44 ToFMMatrix44(const Ogre::Matrix3& mx)
{
	FMMatrix44 fm;
	/*fm[0][0] = mx[0][0]; fm[1][0] = mx[1][0]; fm[2][0] = mx[2][0]; fm[3][0] = mx[3][0];
	fm[0][1] = mx[0][1]; fm[1][1] = mx[1][1]; fm[2][1] = mx[2][1]; fm[3][1] = mx[3][1];
	fm[0][2] = mx[0][2]; fm[1][2] = mx[1][2]; fm[2][2] = mx[2][2]; fm[3][2] = mx[3][2];*/
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			fm[j][i] = mx[i][j];
		}
	}
	//fm[0][3] = fm[1][3] = fm[2][3] = 0.0f; fm[3][3] = 1.0f;
	fm[3][0] = fm[3][1] = fm[3][2] = 0.0f; fm[3][3] = 1.0f;
	return fm;
}
inline FMMatrix44 ToFMMatrix44(const Ogre::Matrix4& mx)
{
	FMMatrix44 fm;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			fm[j][i] = mx[i][j];
		}
	}
	//fm[0][0] = mx[0][0]; fm[1][0] = mx[1][0]; fm[2][0] = mx[2][0]; fm[3][0] = mx[3][0];
	//fm[0][1] = mx[0][1]; fm[1][1] = mx[1][1]; fm[2][1] = mx[2][1]; fm[3][1] = mx[3][1];
	//fm[0][2] = mx[0][2]; fm[1][2] = mx[1][2]; fm[2][2] = mx[2][2]; fm[3][2] = mx[3][2];
	//fm[0][3] = mx[0][3]; fm[1][3] = mx[1][3]; fm[2][3] = mx[2][3]; fm[3][3] = mx[3][3];
	return fm;
}
//
//inline Point3 ToPoint3(const Ogre::Vector3& p) { return Point3(p.x, p.y, p.z); }
//inline Point4 ToPoint4(const FMVector4& p) { return Point4(p.x, p.y, p.z, p.w); }
//inline Color ToColor(const FMVector3& p) { return Color(p.x, p.y, p.z); }
//inline Color ToColor(const FMVector4& p) { return Color(p.x, p.y, p.z); }
//inline AColor ToAColor(const FMVector4& p) { return AColor(p.x, p.y, p.z, p.w); }
//
//inline bool IsEquivalent(const FMVector3& p1, const Point3& p2) { return IsEquivalent(p1.x, p2.x) && IsEquivalent(p1.y, p2.y) && IsEquivalent(p1.z, p2.z); }
//
//inline FMVector3 ToFMVector3(const Point3& p) { return FMVector3(p.x, p.y, p.z); }
//inline FMVector3 ToFMVector3(const Point4& p) { return FMVector3(p.x, p.y, p.z); }
//inline FMVector3 ToFMVector3(const Color& c) { return FMVector3(c.r, c.g, c.b); }
//inline FMVector4 ToFMVector4(const Point4& p) { return FMVector4(p.x, p.y, p.z, p.w); }
//inline FMVector4 ToFMVector4(const Color& c) { return FMVector4(c.r, c.g, c.b, 1.0f); }
//inline FMVector4 ToFMVector4(const AColor& c) { return FMVector4(c.r, c.g, c.b, c.a); }
//
//// added for Point3 properties being converted to FMVector4 in ColladaFX
//inline FMVector4 ToFMVector4(const Point3& p) { return FMVector4(p.x, p.y, p.z, 1.0f); }

#endif // _DOCUMENT_EXPORTER_H_
