// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef WIN32
#pragma warning (disable: 4267 4251 4065 4102)
#endif // WIN32
#ifdef WIN32
#pragma warning( disable: 4190 4204 4996 4482 )

// extern "C" nonsense when returning a template
#endif

#define _CRT_NONSTDC_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#if defined(WIN32) || defined(WIN64) || defined(_WINDOWS)
#include <windows.h>
#endif
//#include <glut.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

#include <stdio.h>
#if defined(WIN32) || defined(WIN64) || defined(_WINDOWS)
    #include <GL/GL.h>
    #include <GL/glu.h>
#else
    #include <OpenGL/GL.h>
    #include <OpenGL/glu.h>
#endif

//#include <OgreNoMemoryMacros.h>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <QFile>//
#include <QHash>
#include <QIcon>
#include <QDir>
#include <QList>
#include <QString>
//#include <QtOpenGL/QGLWidget>
#include <QColor>
#include <QInputDialog>
#include <qdebug.h>
#if defined(WIN32) || defined(WIN64) || defined(_WINDOWS)
#include <QtXml/QDomDocument>
#include <QtGui>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#else
#include <QtXml/QDomDocument>
#include <QtGui>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#endif

#undef uint

//#include <OgreMemoryMacros.h>
#if defined(Q_OS_MAC) && defined(_RELEASE)
#define assert(x) {}
#endif
#include <Ogre.h>

#include <OgreTextAreaOverlayElement.h>
#include "OgreOverlay.h"
#include "OgreOverlayManager.h"
#include "OgrePanelOverlayElement.h"
#include "OgreTextAreaOverlayElement.h"
#include "OgreFontManager.h"
#include "OgreOverlayContainer.h"
#include "OgreOverlayElementFactory.h"

#define OGRE_VERSION_17

#ifndef OGRE_VERSION_17
#include "OgreOverlaySystem.h"
#endif

#include "Core/CoreException.h"
#include "Anima.Core.h"

#ifdef DEBUG_MEMORY
#include "DebugUtils/MemoryRecorder.h"
//#define New DEBUG_NEW
//#define ANIMA_DEL(O) DEBUG_DEL(O)
//#define ANIMA_DEL_ALL DEBUG_DEL_ALL
#define New NewRecorder(__FILE__,__LINE__) ->* new 
#else
#define New new
#endif

#define ANIMA_DEL(O) {__NotifyDelete(O); delete (O);}

template <typename ForwardIterator>
Q_OUTOFLINE_TEMPLATE void ANIMA_DEL_ALL(ForwardIterator begin, ForwardIterator end)
{
    while (begin != end) {
        ANIMA_DEL(*begin);
        ++begin;
    }
}

template <typename Container>
inline void ANIMA_DEL_ALL(const Container &c)
{
    ANIMA_DEL_ALL(c.begin(), c.end());
}



