#pragma once

namespace Anima
{
	class GfxModel;
	class MeshExtModel;
	class MeshExporter;
}

class QTTask;

#define LWCOMMANDFUNC_GLOBAL "LW Command Interface"
typedef int LWCommandFunc ( const char *cmd );

typedef struct st_DBVMapVal {
   char        *name;            // name
   LWID         type;            // type of data (e.g. UVs, weights)
   int          dim;             // dimension (number of values per point)
   void        *vmid;            // LW's opaque vmap ID
   int          nverts;          // number of vertices
   int         *vindex;          // array of vertex indexes
   float      **val;             // 2D array of vmap values
} DBVMap;

typedef struct st_DBVMapPt {
   DBVMap      *vmap;            // where the values are stored
   int          index;           // offset into the vmap value arrays
} DBVMapPt;

typedef struct st_DBPoint {
   LWPntID      id;              // Layout point ID
   LWFVector    pos[ 2 ];        // initial and final position
   int          npols;           // number of polygons sharing the point
   int         *pol;             // array of polygon indexes
   int          nvmaps;          // number of vmap values
   DBVMapPt    *vm;              // array of vmap values
} DBPoint;

typedef struct st_ObjectDB {
   LWItemID     id;              // Layout item ID
   int          npoints;         // number of points
   int          npolygons;       // number of polygons
   int          nsurfaces;       // number of surfaces
   int          nvertmaps;       // number of vertex maps
   DBPoint     *pt;              // point array
   int         *vsort;           // point indexes sorted by point position
   int          vsortkey;        // coordinate for position sort
   //DBPolygon   *pol;             // polygon array
   //DBSurface   *surf;            // surface array
   //DBVMap      *vmap;            // vmap array
} ObjectDB;

class AnimaStore
{
private:

	GlobalFunc *global;
	char programPath[2000];
	bool initialized;

	ObjectDB *lwObject;

	QHash<LWPntID,int> *pointMap;
	
	//int baseVertexCount;
	int totalVertexCount;
	//Ogre::Vector3* TotalVertexArray;
	int currentVertex;

	QTTask *qtTask;

private:

	Anima::MeshExporter *meshExporter;

private:
	//int pntScan( ObjectDB *odb, LWPntID id );
	int EnsureInit();
	int LoadSurf( const char *filename,int clipIdx,QString &txtMap,unsigned char *buf,int fileSize,int *startBufferIdx);
	void AddMaterials(LWObjectImport *local,Anima::MeshExporter &_meshExporter);

public:

	AnimaStore(GlobalFunc *global,char *programPath);
	~AnimaStore();

	void Clear();
	void NotifyModel(LWDisplacementHandler *local,LWMeshInfo *meshInfo,int *startVertexIdx,Anima::MeshExtModel *meshExt);
	void AddModel(LWObjectImport *local,Anima::MeshExtModel *model);

	// load
	int Load( LWObjectImport *local,void *serverData);
	
	// displacement
	int DisplacementInit( LWDisplacementHandler *local, void *serverData);
	void DisplaceLWPoint( AnimaData *dat, LWDisplacementAccess *da );
	void NewTime( AnimaData *dat, LWFrame fr, LWTime t,double fps  );

	void OnDrawModel(Anima::GfxModel *model);

};
