#############################################################################
# Makefile for building: libAnimaCore.1.0.0.dylib
# Generated by qmake (2.01a) (Qt 4.6.3) on: Thu Jul 14 16:44:21 2011
# Project:  Anima.Core.pro
# Template: lib
# Command: /usr/bin/qmake -spec /usr/local/Qt4.6/mkspecs/macx-xcode -macx -o AnimaCore.xcodeproj/project.pbxproj Anima.Core.pro
#############################################################################

MOC       = /Developer/Tools/Qt/moc
UIC       = /Developer/Tools/Qt/uic
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
INCPATH       = -I/usr/local/Qt4.6/mkspecs/macx-xcode -I. -I/Library/Frameworks/QtCore.framework/Versions/4/Headers -I/usr/include/QtCore -I/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/usr/include/QtGui -I/Library/Frameworks/QtXml.framework/Versions/4/Headers -I/usr/include/QtXml -I/usr/include -I. -ICore -IDebugUtils -ICore/Edit -ICore/Action -ICore/Action/ActionModes -ICore/AnimParts -IGfx -IGfx/Render -IPhysic -ITask -IImport/IO -ICore/Modders -IUI -ICore/IA -ICore/IA/OpenSteer -ICore/PathGfxBuilders -IExport -ICore/Command -IImport -IOgre -IGfx/GL -I../Anima.Import -I../../lib/Ogre.framework/Headers -I../../lib/boost_1_46_1 -I. -I/usr/local/include -I/System/Library/Frameworks/CarbonCore.framework/Headers -F/Library/Frameworks
DEL_FILE  = rm -f
MOVE      = mv -f

IMAGES = 
PARSERS =
preprocess: $(PARSERS) compilers
clean preprocess_clean: parser_clean compiler_clean

parser_clean:
check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compilers: ./moc_Context.cpp ./moc_Film.cpp ./moc_VideoManager.cpp\
	 ./moc_TaskQueue.cpp ./moc_GUIManager.cpp ./moc_CommandManager.cpp\
	 ./moc_EditorManager.cpp ./moc_EventManager.cpp ./moc_SelectionManager.cpp\
	 ./moc_PathDesignMode.cpp
compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all: moc_Context.cpp moc_Film.cpp moc_VideoManager.cpp moc_TaskQueue.cpp moc_GUIManager.cpp moc_CommandManager.cpp moc_EditorManager.cpp moc_EventManager.cpp moc_SelectionManager.cpp moc_PathDesignMode.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_Context.cpp moc_Film.cpp moc_VideoManager.cpp moc_TaskQueue.cpp moc_GUIManager.cpp moc_CommandManager.cpp moc_EditorManager.cpp moc_EventManager.cpp moc_SelectionManager.cpp moc_PathDesignMode.cpp
moc_Context.cpp: stdafx.h \
		Core/CoreException.h \
		Anima.Core.h \
		DebugUtils/MemoryRecorder.h \
		Anima_nsbegin.h \
		Anima_nsend.h \
		Core/Edit/SelectionManager.h \
		Core/Context.h \
		Core/Context.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Core/Context.h -o moc_Context.cpp

moc_Film.cpp: Anima_nsbegin.h \
		Anima_nsend.h \
		Core/Film.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Core/Film.h -o moc_Film.cpp

moc_VideoManager.cpp: Anima_nsbegin.h \
		Anima_nsend.h \
		Export/VideoManager.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Export/VideoManager.h -o moc_VideoManager.cpp

moc_TaskQueue.cpp: Anima_nsbegin.h \
		Anima_nsend.h \
		Task/TaskQueue.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Task/TaskQueue.h -o moc_TaskQueue.cpp

moc_GUIManager.cpp: Anima_nsbegin.h \
		Anima_nsend.h \
		UI/GUIManager.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ UI/GUIManager.h -o moc_GUIManager.cpp

moc_CommandManager.cpp: Core/Command/Command.h \
		Anima_nsbegin.h \
		Anima_nsend.h \
		Core/Edit/EditorManager.h \
		Core/Mesh.h \
		Core/SceneObject.h \
		Core/ResourceManager.h \
		Core/DAG.h \
		Anima.Core.h \
		Core/Action/ActionModes/SplineDesignMode.h \
		Core/Action/ActionModes/PathDesignMode.h \
		Core/Animation.h \
		Core/SceneLibrary.h \
		Core/SceneObjectMaterial.h \
		Core/AnimationEvent.h \
		Core/MovePath.h \
		Core/SceneObjectGfx.h \
		Core/SceneObjectGfxMesh.h \
		Core/BuildDispatcher.h \
		Core/Edit/SelectionManager.h \
		Core/Context.h \
		stdafx.h \
		Core/CoreException.h \
		DebugUtils/MemoryRecorder.h \
		Core/Command/CommandManager.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Core/Command/CommandManager.h -o moc_CommandManager.cpp

moc_EditorManager.cpp: Core/Mesh.h \
		Core/SceneObject.h \
		Core/ResourceManager.h \
		Anima_nsbegin.h \
		Anima_nsend.h \
		Core/DAG.h \
		Anima.Core.h \
		Core/Action/ActionModes/SplineDesignMode.h \
		Core/Action/ActionModes/PathDesignMode.h \
		Core/Edit/EditorManager.h \
		Core/Animation.h \
		Core/SceneLibrary.h \
		Core/SceneObjectMaterial.h \
		Core/AnimationEvent.h \
		Core/MovePath.h \
		Core/SceneObjectGfx.h \
		Core/SceneObjectGfxMesh.h \
		Core/BuildDispatcher.h \
		Core/Edit/EditorManager.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Core/Edit/EditorManager.h -o moc_EditorManager.cpp

moc_EventManager.cpp: Anima_nsbegin.h \
		Anima_nsend.h \
		Core/Edit/EventManager.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Core/Edit/EventManager.h -o moc_EventManager.cpp

moc_SelectionManager.cpp: Core/Context.h \
		stdafx.h \
		Core/CoreException.h \
		Anima.Core.h \
		DebugUtils/MemoryRecorder.h \
		Anima_nsbegin.h \
		Anima_nsend.h \
		Core/Edit/SelectionManager.h \
		Core/Edit/SelectionManager.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Core/Edit/SelectionManager.h -o moc_SelectionManager.cpp

moc_PathDesignMode.cpp: Core/Edit/EditorManager.h \
		Core/Mesh.h \
		Core/SceneObject.h \
		Core/ResourceManager.h \
		Anima_nsbegin.h \
		Anima_nsend.h \
		Core/DAG.h \
		Anima.Core.h \
		Core/Action/ActionModes/SplineDesignMode.h \
		Core/Action/ActionModes/PathDesignMode.h \
		Core/Animation.h \
		Core/SceneLibrary.h \
		Core/SceneObjectMaterial.h \
		Core/AnimationEvent.h \
		Core/MovePath.h \
		Core/SceneObjectGfx.h \
		Core/SceneObjectGfxMesh.h \
		Core/BuildDispatcher.h \
		Core/Action/ActionModes/PathDesignMode.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Core/Action/ActionModes/PathDesignMode.h -o moc_PathDesignMode.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 
