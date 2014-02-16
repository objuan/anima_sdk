#pragma once
#if API_VERSION < 11600
#define BaseVideoPost PluginVideoPost
#define BaseVideoPost PluginVideoPost
#define BaseShader PluginShader

#define DateTime tagDateTime
#define GetDateTimeNow DateTimeNow
#define MAX_MATERIALCHANNELS MAXCHANNELS
#define STRINGENCODING_UTF8 StUTF8
#define EXECUTIONRESULT Int32
#define EXECUTIONRESULT_OK EXECUTION_RESULT_OK
#define EXECUTIONFLAGS Int32
#define INITRENDERRESULT Int32
#define INITRENDERRESULT_OK 0
#define INITRENDERRESULT_ASSETMISSING LOAD_NOTFOUND
#define Lock LockAndWait
#define Unlock UnLock
#define IMAGERESULT_OK IMAGE_OK
#define DESCFLAGS_SET Int32
#define DESCFLAGS_GET Int32
#define DESCFLAGS_DESC Int32
#define DESCFLAGS_DESC_0 0
#define DESCFLAGS_SET_0 0
#define DESCFLAGS_GET_0 0
#define DESCFLAGS_SET_PARAM_SET DESCFLAGS_PARAM_SET
#define DESCFLAGS_GET_PARAM_GET DESCFLAGS_PARAM_GET
#define DESCFLAGS_ENABLE Int32
#define FILEOPEN_WRITE GE_WRITE
#define MAXREALr MAXREAL
#define RAYBIT_0 0
#define RAYBIT_REFLECTION RAY_REFLECTION
#define RAYBIT_REFRACTION RAY_REFRACTION
#define RAYBIT_CURR_REFLECTION RAY_CURR_REFLECTION
#define RAYBIT_CURR_TRANSPARENCY RAY_CURR_TRANSPARENCY
#define RAYBIT_CURR_REFRACTION RAY_CURR_REFRACTION

#define HDIRTYFLAGS_ALL -1
#define DIRTYFLAGS_ALL -1

#define RENDERRESULT Int32
#define RENDERRESULT_USERBREAK RAY_USERBREAK
#define RENDERRESULT_OK RAY_OK
#define RENDERRESULT_ASSETMISSING RAY_IMAGE
#define RENDERRESULT_OUTOFMEMORY RAY_NOMEM
#define RENDERFLAGS Int32
#define RENDERFLAGS_EXTERNAL RENDERFLAG_EXTERNAL
#define RENDERFLAGS_PREVIEWRENDER RENDERFLAG_PREVIEWRENDER
#define RENDERFLAGS_NODOCUMENTCLONE RENDERFLAG_NODOCUMENTCLONE
#define RENDERFLAGS_IRR RENDERFLAG_IRR

#define VIDEOPOSTCALL_FRAMESEQUENCE VP_FRAMESEQUENCE
#define VIDEOPOSTCALL_FRAME VP_FRAME
#define VIDEOPOSTCALL_SUBFRAME VP_SUBFRAME
#define VIDEOPOSTCALL_RENDER VP_RENDER
#define VIDEOPOSTCALL_INNER VP_INNER

#define VIDEOPOSTINFO Int32
#define VIDEOPOSTINFO_0 0
#define VIDEOPOSTINFO_REQUEST_MOTIONMATRIX VIDEOPOST_REQUEST_MOTIONMATRIX
#define VIDEOPOSTINFO_REQUEST_MOTIONGEOMETRY VIDEOPOST_REQUEST_MOTIONGEOMETRY

#define COPYFLAGS Int32
#define COPYFLAGS_0 0
#define COPYFLAGS_NO_HIERARCHY COPY_NO_HIERARCHY
#define SERIALINFO_MULTILICENSE SERIAL_MULTILICENSE
#define SERIALINFO_CINEMA4D SERIAL_CINEMA4D
#define COLORMODE_RGB MODE_RGB
#define VERSIONTYPE_NET_CLIENT VERSION_NET
#define VERSIONTYPE_NET_SERVER_3 VERSION_SERVER
#define VERSIONTYPE_NET_SERVER_UNLIMITED VERSION_SERVER

#define DIRTYFLAGS_DATA DIRTY_DATA
#define DIRTYFLAGS_MATRIX DIRTY_MATRIX
#define DIRTYFLAGS_CACHE DIRTY_CACHE
#define MSG_GETALLASSETS MSG_GETALLSTRINGS

#define FILESELECTTYPE_ANYTHING FSTYPE_ANYTHING
#define FILESELECTTYPE_SCENES FSTYPE_SCENES

#define AssetData GetAllStringData

#define GetGuiWorldColor GetWorldColor
#define DrawResultDrawn TRUE
#define DRAWRESULT Bool
#define DRAWRESULT_DRAWN TRUE
#define DRAWPASS Int32

#define MODELINGCOMMANDMODE_POLYGONSELECTION MODIFY_POLYGONSELECTION
#define MODELINGCOMMANDMODE_POINTSELECTION MODIFY_POINTSELECTION
#define MODELINGCOMMANDMODE_ALL MODIFY_ALL

#define SPLINETYPE_LINEAR Tlinear
#define DRAWHANDLE_BIG HANDLE_BIG
#define UNDOTYPE_NEW UNDO_NEW
#define UNDOTYPE_CHANGE UNDO_CHANGE
#define UNDOTYPE_DELETE UNDO_DELETE

#define OPERATINGSYSTEM_OSX GE_MAC
#define FILEOPEN_READ GE_READ
#define FILEOPEN_APPEND GE_APPEND
#define FILEDIALOG_ANY FILE_DIALOG
#define FILEDIALOG_NONE FILE_NODIALOG
#define BYTEORDER_INTEL GE_INTEL
#define FILESELECT_LOAD 0
#define SERIALINFO_CINEMA4D SERIAL_CINEMA4D
#define GETBRANCHINFO_0 0
#define DLG_TYPE_ASYNC TRUE
#define THREADMODE_ASYNC TRUE
#define THREADMODE_SYNCHRONOUS FALSE
#define THREADPRIORITY_NORMAL 0
#define EXECUTIONPRIORITY_INITIAL EXECUTION_INITIAL
#define COLORMODE_RGB MODE_RGB
#define PIXELCNT_0 0
#define COLORBYTES_GRAY       1
#define COLORBYTES_AGRAY      2
#define COLORBYTES_RGB        3
#define COLORBYTES_ARGB       4
#define COLORBYTES_CMYK       4
#define COLORBYTES_ACMYK      5
#define SAMPLEBUMP Int32
#define AutoBitmap(x) x
#define BUILDFLAGS_INTERNALRENDERER VFLAG_INTERNALRENDERER
#define BUILDFLAGS_EXTERNALRENDERER VFLAG_EXTERNALRENDERER
#define GetBuildFlags GetVFlags
#define SetAbsPos SetPos
#define UVWHandle void*
#define EVENT_0 0
#define SHADERINFO Int32
#define SHADERINFO_0 0
#define MAXREALs MAXREAL
#define EDGESELECTIONTYPE_PHONG EDGES_PHONG
#define OPERATINGSYSTEM_WIN GE_WIN
#endif