#pragma once
#ifdef WIN32
#pragma warning (disable: 4267 4251 4065 4102)
#endif // WIN32
#ifdef WIN32
#pragma warning( disable: 4190 4204 4996 4482)
// extern "C" nonsense when returning a template
#endif

#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <QFile>
#include <QHash>
#include <QIcon>
#include <QDir>
#include <QList>
#include <QString>
#include <QDomDocument>
//#include <QtOpenGL/QGLWidget>
#include <QColor>
#include <QtGui>
#include <QInputDialog>
#include <qdebug.h>

#ifdef  C4D_R13

#include <Ogre.h>

#ifndef _WIN32
#define ANIMA_CORE_API
#endif

#include "Anima.Core.h"

//namespace Anima
//{
#include "Core/Context.h"
#include "Core/Film.h"
#include "Core/FilmLayer.h"
#include "Core/Animation.h"
#include "Core/ModelInstance.h"
#include "Core/Model.h"
#include "Core/ModelLibrary.h"
#include "Core/Config.h"
#include "Task/TaskQueue.h"
#include "UI/GUIManager.h"
#include "Core/ResourceManager.h"
#include "Gfx/GfxContext.h"
#include "Gfx/GfxPerformanceManager.h"
#include "Gfx/GfxBaseModel.h"
#include "Import/IO/ContextIO.h"
#include "Export/MeshExporter.h"
#include "Export/ParamsExporter.h"
#include "Export/MeshExt.h"
#include "Export/PresetsExporter.h"
//}

#define LONG Int32
#define GetFloat GetReal
#define SetFloat SetReal
#define GetInt32 GetLong
#define SetInt32 SetLong
#define VInt32 LLONG

#else

#include "c4d.h"
#undef EPSILON
#undef override

#include <Ogre.h>

#ifndef _WIN32
#define ANIMA_CORE_API
#endif

#include "Anima.Core.h"

//namespace Anima
//{
#include "Core/Context.h"
#include "Core/Film.h"
#include "Core/FilmLayer.h"
#include "Core/Animation.h"
#include "Core/ModelInstance.h"
#include "Core/Model.h"
#include "Core/ModelLibrary.h"
#include "Core/Config.h"
#include "Task/TaskQueue.h"
#include "UI/GUIManager.h"
#include "Core/ResourceManager.h"
#include "Gfx/GfxContext.h"
#include "Gfx/GfxPerformanceManager.h"
#include "Gfx/GfxBaseModel.h"
#include "Import/IO/ContextIO.h"
#include "Export/MeshExporter.h"
#include "Export/ParamsExporter.h"
#include "Export/MeshExt.h"
#include "Export/PresetsExporter.h"
//}

String LongToString(Int32 a);

#define gDelete delete
#define gNew new
#define VInt32 Int64
#define MAXREALs 999999.0f
#define Real float
#define SReal float
#endif
