# macros

!IF $(VER) == 64
VERSION = \x64
MACHINE = X64
VERSION_DEFINE = "WIN64"
TARGET_DIR_O = ../
!ELSE
VERSION = 
MACHINE = X86
VERSION_DEFINE = "WIN32"
TARGET_DIR_O =
!endif	

DEBUG_MODE = 1
MAX_LIB = comctl32.lib core.lib geom.lib gfx.lib helpsys.lib maxutil.lib mesh.lib

!IF $(MAX) == 9
MAX_SDK_INC = "$(MAX_SDK)\include"
MAX_SDK_LIB = "$(MAX_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.MAX9
MAX_DEFINES = MAX9
!ELSEIF $(MAX) == 2009
MAX_SDK_INC = "$(MAX2009_SDK)\include"
MAX_SDK_LIB = "$(MAX2009_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.MAX2009
MAX_DEFINES = MAX2009
!ELSEIF $(MAX) == 2008
MAX_SDK_INC = "$(MAX2008_SDK)\include"
MAX_SDK_LIB = "$(MAX2008_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.MAX2008
MAX_DEFINES = MAX2008
!ELSEIF $(MAX) == 2010
MAX_SDK_INC = "$(MAX2010_SDK)\include"
MAX_SDK_LIB = "$(MAX2010_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.MAX2010
MAX_DEFINES = MAX2010
!ELSEIF $(MAX) == 2011
MAX_SDK_INC = "$(MAX2011_SDK)\include"
MAX_SDK_LIB = "$(MAX2011_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.MAX2011
MAX_DEFINES = MAX2011
!ELSEIF $(MAX) == 2012
MAX_SDK_INC = "$(MAX2012_SDK)\include"
MAX_SDK_LIB = "$(MAX2012_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.MAX2012
MAX_DEFINES = MAX2012
MAX_LIB = core.lib geom.lib gfx.lib maxutil.lib mesh.lib
!ELSEIF $(MAX) == 2013
MAX_SDK_INC = "$(MAX2013_SDK)\include"
MAX_SDK_LIB = "$(MAX2013_SDK)$(VERSION)\lib"
PRE_TARGET_NAME = Anima.MAX2013
MAX_DEFINES = MAX2013
MAX_LIB = core.lib geom.lib gfx.lib maxutil.lib mesh.lib
!ELSEIF $(MAX) == 2014
MAX_SDK_INC = "$(MAX2014_SDK)\include"
MAX_SDK_LIB = "$(MAX2014_SDK)\lib$(VERSION)\release"
PRE_TARGET_NAME = Anima.MAX2014
MAX_DEFINES = MAX2014
MAX_LIB = core.lib geom.lib gfx.lib maxutil.lib mesh.lib
!else				
% abort Unsupported MAX = $(MAX)
!endif	

!IF $(VER) == 64
TARGET_NAME = $(PRE_TARGET_NAME)_x64
!ELSE
TARGET_NAME = $(PRE_TARGET_NAME)
!endif	

TARGET_DIR = $(TARGET_DIR_O)../../../../../bin/plugins$(VERSION)
BIN_DIR = ./bin$(VERSION)
OBJ_DIR = ./obj$(VERSION)
LIB_DIR = ../../../../lib$(VERSION)
#WRK_DIR = ../import/
WRK_DIR = ..
ANIMA_DIR = ../../../../

# ======

LIST_DLI   	= $(OBJ_DIR)\AnimaImp.obj

# ======

# non usati  /Fo"$(OBJ_DIR)/"  \
# /Fp"$(OBJ_DIR)/$(TARGET_NAME).pch" \
#/Fd"$(TARGET_DIR)/$(TARGET_NAME).dll.pdb" \

# DEBUG /Zi , togliere per la produzione
 
DEFINES = /D $(VERSION_DEFINE) /D $(MAX_DEFINES) /D "_SECURE_SCL=0" /D "NDEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_NONSTDC_NO_DEPRECATE" /D "_SCL_SECURE_NO_DEPRECATE" /D "ISOLATION_AWARE_ENABLED=1" /D "_WINDLL"
DEFINES_UNICODE = /D "_UNICODE" /D "_WINDLL" /D "UNICODE"
INCLUDES = /I $(MAX_SDK_INC) /I ".\midl\Win32" /I "..\..\include" 
OPTIMIZE = /O2 /Ob2 /Oi /Ot /GT 

CFLAGS   = $(INCLUDES) $(DEFINES) $(DEFINES_UNICODE) $(OPTIMIZE) /GF /FD /EHa /MD /GS- /Gy /arch:SSE /fp:fast \
			/Fo"$(OBJ_DIR)/"  \
			 /W3 /nologo /c /FC /TP /errorReport:prompt

# AUS
# /IMPLIB:"$(LIB_DIR)\$(TARGET_NAME).lib"  \
#

LFLAGS   =  /OUT:"$(BIN_DIR)/$(TARGET_NAME).dli" \
			/INCREMENTAL:NO /NOLOGO  \
			/LIBPATH:$(MAX_SDK_LIB) \
			/DLL \
			/MANIFEST /MANIFESTFILE:"$(OBJ_DIR)\$(TARGET_NAME).dli.intermediate.manifest" /MANIFESTUAC:"level='asInvoker' uiAccess='false'" \
			/NODEFAULTLIB:"libcp.lib" /NODEFAULTLIB:"libci.lib" /NODEFAULTLIB:"msvcirt.lib" /NODEFAULTLIB:"libcmt.lib" /NODEFAULTLIB:"libcmtd.lib" /NODEFAULTLIB:"msvcrtd.lib" \
			/DEF:"$(WRK_DIR)\animaimp.def" \
			/DEBUG \
			/PDB:"$(BIN_DIR)/$(TARGET_NAME).dli.pdb" \
			/SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF  /RELEASE /DYNAMICBASE:NO \
			/MACHINE:$(MACHINE) \
			/ERRORREPORT:PROMPT  \
			$(MAX_LIB) \
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

$(TARGET_NAME).dli : $(LIST_DLI)
	link $(LFLAGS) $(**R)
	mt.exe /outputresource:"$(BIN_DIR)/$(TARGET_NAME).dli;#2" \
		/manifest "$(OBJ_DIR)\$(TARGET_NAME).dli.intermediate.manifest" /nologo
	cd "$(BIN_DIR)"
	copy "$(*F).dli" "$(TARGET_DIR)/$(*F).dli"
	copy "$(*F).dli.pdb" "$(TARGET_DIR)/$(*F).dli.pdb"
	cd ".."
	
$(OBJ_DIR)\*.obj : $(WRK_DIR)\$$(@B).cpp 

