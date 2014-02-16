#pragma once

#ifdef ANIMA_MAX_EXPORTS
#define ANIMA_MAX_EXPORT __declspec(dllexport)
#else
#define ANIMA_MAX_EXPORT __declspec(dllimport)
#endif

int __stdcall
MAX_API_DoImport(const char*programPath,const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts=FALSE);

ClassDesc *__stdcall MAX_API_AnimaObjectClass(HINSTANCE hInstance,const char*programPath);

ClassDesc *__stdcall MAX_API_AnimaObjectSubMeshClass(HINSTANCE hInstance);

ClassDesc *__stdcall MAX_API_AnimaObjectDummyClass(HINSTANCE hInstance);

ClassDesc *__stdcall MAX_API_AnimaObjectGeomClass(HINSTANCE hInstance);
