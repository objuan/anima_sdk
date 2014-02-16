// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef WIN32
#pragma warning (disable: 4267 4251 4065 4102)
#endif // WIN32
#ifdef WIN32
#pragma warning( disable: 4190 4204)
// extern "C" nonsense when returning a template
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <lwserver.h>
#include <lwobjimp.h>
#include <lwsurf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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
#include <QGLWidget>
#include <QColor>
#include <QtGui>
#include <QInputDialog>
#include <qdebug.h>

#include <fbxsdk.h>
#include <fbxfilesdk_nsuse.h>

#include "Core/Context.h"
#include "Core/Film.h"
#include "Core/FilmLayer.h"
#include "Core/Animation.h"
#include "Core/ModelInstance.h"
#include "Core/Model.h"
#include "Core/ModelLibrary.h"
#include "Core/Config.h"
#include "Core/ResourceManager.h"
#include "Gfx/GfxContext.h"
#include "Gfx/GfxModel.h"
#include "IO/ContextIO.h"



