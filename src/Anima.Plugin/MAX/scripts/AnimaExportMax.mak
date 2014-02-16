# macros

MAX_LIB = bmm.lib comctl32.lib core.lib paramblk2.lib geom.lib gfx.lib helpsys.lib maxutil.lib mesh.lib

!IF $(VER) == 64
VERSION = \x64
MACHINE = X64
VERSION_DEFINE = "WIN64"
TARGET_DIR_O = ../
ANIMA_LIB = Anima.Core_x64.lib SharedUtilities.lib Anima.Studio.lib
QT_LIB = QtGuiAnima_x64_4.lib QtCoreAnima_x64_4.lib QtXmlAnima_x64_4.lib
!ELSE
VERSION = 
MACHINE = X86
VERSION_DEFINE = "WIN32"
TARGET_DIR_O =
ANIMA_LIB = Anima.Core.lib SharedUtilities.lib Anima.Studio.lib
QT_LIB = QtGuiAnima4.lib QtCoreAnima4.lib QtXmlAnima4.lib
!endif	

TARGET_DIR = $(TARGET_DIR_O)../../../../../bin/$(VERSION)
BIN_DIR = ./bin$(VERSION)
OBJ_DIR = ./obj$(VERSION)
LIB_DIR = ../../../../lib$(VERSION)
WRK_DIR = ../Exporter/AnimaExporter/Sources
ANIMA_DIR = ../../../../

DEBUG_MODE = 1

!IF $(MAX) == 9
MAX_SDK_INC = "$(MAX_SDK)\include"
MAX_SDK_LIB = "$(MAX_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.Export.MAX9
MAX_DEFINES = MAX9
!ELSEIF $(MAX) == 2008
MAX_SDK_INC = "$(MAX2008_SDK)\include"
MAX_SDK_LIB = "$(MAX2008_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.Export.MAX2008
MAX_DEFINES = MAX2008
!ELSEIF $(MAX) == 2009
MAX_SDK_INC = "$(MAX2009_SDK)\include"
MAX_SDK_LIB = "$(MAX2009_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.Export.MAX2009
MAX_DEFINES = MAX2009
!ELSEIF $(MAX) == 2010
MAX_SDK_INC = "$(MAX2010_SDK)\include"
MAX_SDK_LIB = "$(MAX2010_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.Export.MAX2010
MAX_DEFINES = MAX2010
!ELSEIF $(MAX) == 2011
MAX_SDK_INC = "$(MAX2011_SDK)\include"
MAX_SDK_LIB = "$(MAX2011_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.Export.MAX2011
MAX_DEFINES = MAX2011
!ELSEIF $(MAX) == 2012
MAX_SDK_INC = "$(MAX2012_SDK)\include"
MAX_SDK_LIB = "$(MAX2012_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.Export.MAX2012
MAX_DEFINES = MAX2012
MAX_LIB = bmm.lib core.lib paramblk2.lib geom.lib gfx.lib maxutil.lib mesh.lib
!else				
% abort Unsupported MAX = $(MAX)
!endif	

!IF $(VER) == 64
TARGET_NAME = $(PRE_TARGET_NAME)_x64
!ELSE
TARGET_NAME = $(PRE_TARGET_NAME)
!endif	

# ======

LIST_MAX_DLL   = $(OBJ_DIR)\LexiStdAfx.obj $(OBJ_DIR)\AnimaDialogProgress.obj $(OBJ_DIR)\AnimaExporter.obj $(OBJ_DIR)\AnimaExportObject.obj \
				$(OBJ_DIR)\DLLMain.obj $(OBJ_DIR)\MAXIntermediateBuilderSkeleton.obj $(OBJ_DIR)\MAXIntermediateMesh.obj  \
				$(OBJ_DIR)\MAXntermediateBuilder.obj  $(OBJ_DIR)\MAXRendererAPI.obj $(OBJ_DIR)\qrc__ico_res.obj 

				
# ======	
	
# non usati  /Fo"$(OBJ_DIR)/"  \
# /Fp"$(OBJ_DIR)/$(TARGET_NAME).pch" \
#/Fd"$(TARGET_DIR)/$(TARGET_NAME).dll.pdb" \

DEFINES = /D $(MAX_DEFINES)  /D $(MAX_DEFINES) /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_DEPRECATE" \
		 /D "_CRT_NONSTDC_NO_DEPRECATE" /D "_SCL_SECURE_NO_DEPRECATE" /D "ISOLATION_AWARE_ENABLED=1" /D "_WINDLL" \
		 /D "ANIMA_NAMESPACE_USE" /D "ANIMA_MAX_EXPORTS"
INCLUDES =  /I "$(ANIMA_DIR)/contrib\ogre\x32" \
			/I $(WRK_DIR)/../Res \
			/I "$(OGRE_HOME)\include" \
			/I "$(QTDIR)\include\QtXml" /I "$(QTDIR)\include\QtCore" /I "$(QTDIR)\include\QtGui" /I "$(QTDIR)\include\QtOpenGL" /I  "$(QTDIR)\include" /I "$(QTDIR)\include\ActiveQt" \
			/I "$(ANIMA_DIR)/src/Anima.Core" \
			/I "$(ANIMA_DIR)/src/Anima.Studio" \
			/I "$(ANIMA_DIR)/src/Anima.Core/core" \
			/I $(MAX_SDK_INC) \
			/I $(WRK_DIR)/../../include
		
OPTIMIZE = /O2 /Ob2 /Oi /Ot /GT 
CFLAGS   = $(INCLUDES) $(DEFINES) $(OPTIMIZE) /GF /FD /Fd"$(OBJ_DIR)\vc90.pdb" /EHa /MD /GS- /Gy /arch:SSE /fp:fast \
			/Fo"$(OBJ_DIR)/"  \
			/W3 /nologo /c /FC /TP /errorReport:prompt

# AUS
# /IMPLIB:"$(LIB_DIR)\$(TARGET_NAME).lib"  \
#

LFLAGS   =  /OUT:"$(BIN_DIR)/$(TARGET_NAME).dll" \
			/INCREMENTAL:NO /NOLOGO  \
			/LIBPATH:"$(LIB_DIR)" \
			/LIBPATH:$(MAX_SDK_LIB) \
			/DLL \
			/MANIFEST /MANIFESTFILE:"$(OBJ_DIR)\$(TARGET_NAME).dll.intermediate.manifest" /MANIFESTUAC:"level='asInvoker' uiAccess='false'" \
			/NODEFAULTLIB:"libcp.lib" /NODEFAULTLIB:"libci.lib" /NODEFAULTLIB:"msvcirt.lib" /NODEFAULTLIB:"libcmt.lib" /NODEFAULTLIB:"libcmtd.lib" /NODEFAULTLIB:"msvcrtd.lib" \
			/DEBUG \
			/PDB:"$(BIN_DIR)/$(TARGET_NAME).dll.pdb" \
			/SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF  /RELEASE /DYNAMICBASE:NO \
			/MACHINE:$(MACHINE) \
			/ERRORREPORT:PROMPT  \
			$(MAX_LIB) \
			$(QT_LIB) OgreMain.lib $(ANIMA_LIB) \
			kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
			
# inference rules

# compile
# $< represents the dependent file.
# $(*F) represents the targets base name.
# $(*R) represents the targets base name and directory.

{$(WRK_DIR)}.cpp{$(OBJ_DIR)}.obj:
   cl $(CFLAGS) $<

# link
# $(**R) represents all the dependents including directories.

# target-dependencies
# $$(@B) represents the base name of the current target.
$(TARGET_NAME).dll : $(LIST_MAX_DLL)
    rc /d "_VC80_UPGRADE=0x0710" /fo"$(OBJ_DIR)\resource.res" $(WRK_DIR)\..\Res\LexiExport.rc"
	link $(LFLAGS) $(**R) $(OBJ_DIR)\resource.res"
	mt.exe /outputresource:"$(BIN_DIR)/$(TARGET_NAME).dll;#2" \
		/manifest "$(OBJ_DIR)\$(TARGET_NAME).dll.intermediate.manifest" /nologo
	cd "$(BIN_DIR)"
	copy "$(*F).dll" "$(TARGET_DIR)/$(*F).dll"
	copy "$(*F).dll.pdb" "$(TARGET_DIR)/$(*F).dll.pdb"
	cd ".."
	
$(OBJ_DIR)\*.obj : $(WRK_DIR)\$$(@B).cpp 