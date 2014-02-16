/*
======================================================================
pipeline.h

Plug-in names and trace function prototype.

Ernie Wright  15 Jul 01
====================================================================== */

#include <lwserver.h>
#include <stdio.h>
#include <stdlib.h>

#define MAST_PNAME "Anima_Master"
/*
#define LIMIT_SHADOW_PNAME "Stack_LimitShadow"
#define STACK_LIMITSHADOW  "Stack_LimitShadow"
#define CUTSHADOW_PNAME "Stack_CutShadow"
*/
typedef int (*LWCommandFunc)(const char *);

void trace( const char *func, const char *plug, const char *fmt, ... );

