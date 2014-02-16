// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef WIN32
#pragma warning (disable: 4267 4251 4065 4102)
#endif // WIN32
#ifdef WIN32
#pragma warning( disable: 4190 4204 4996 4482)
// extern "C" nonsense when returning a template
#endif

#define _CRT_NONSTDC_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:

//#if defined(Q_WS_WIN)
#include <windows.h>	// needed for WindowFromDC()
//#else
//#include <Qt/qx11info_x11.h>
//#include <X11/Xlib.h>
//#endif
//#include <glut.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <GL/GL.h>
#include <gl/glu.h>

//#include <OgreNoMemoryMacros.h>
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
//#include <OgreMemoryMacros.h>

#include <Ogre.h>

#ifndef QT_BEGIN_NAMESPACE
#define QT_BEGIN_NAMESPACE
#endif
#ifndef QT_END_NAMESPACE
#define QT_END_NAMESPACE
#endif

#include <lwobjimp.h>
#include <lwserver.h>
#include <lwsurf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <lwdisplce.h>
#include <lwhost.h>
#include <lwhandler.h>
#include <lwmeshes.h>
#include <lwdisplce.h>
#include <lwenvel.h>
#include <lwvparm.h>

#include "Anima.Core.h"

// --------
