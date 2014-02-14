#pragma once

#include "./Mesh.h"

#include "Anima_nsbegin.h"
//class MeshKey
//{
//public:
//	QString name;
//	MeshParams pars;
//
//	MeshKey(const QString &name,MeshParams pars) :name(name), pars(pars)
//	{
//	}
//};
//
//inline bool operator==(const MeshKey &e1, const MeshKey &e2)
//{
//    return e1.name == e2.name && e1.pars==e2.pars;
//}
//
//inline uint qHash(const MeshKey &key)
//{
//	return (uint)( qHash(key.name) + qHash(key.pars));
//}

class DAG;

// =====================================================

enum MeshLibrary_MeshType
{
	MeshLibrary_MeshType_Normal = 0,
	MeshLibrary_MeshType_TopMost = 1,
	MeshLibrary_MeshType_LayerColor = 2
};
	
class ANIMA_CORE_API MeshLibrary : public BObject
{
private:

	QHash<QString,IMesh *> meshLibrary;
	QHash<int,QString> selectedDagMap;
	int selectedDagIdx;

	void Register(const QString &name,IMesh *mesh);
	
public:

	static MeshLibrary Singleton;

private:

	MeshLibrary();
	~MeshLibrary();

	MeshStatic *AddStaticMesh(const QString &name,const QString &path,MeshLibrary_MeshType type);
//	MeshEditable *AddEditableMesh(const QString &name,const QString &path,MeshLibrary_MeshType type);

public:

	void Load();

	void BeginSelection();
	int AddSelectionDAG(DAG &dag);
	DAG GetSelectionDAG(int idx);

	IMesh *GetMesh(const QString &name); 
	

};
#include "Anima_nsend.h"