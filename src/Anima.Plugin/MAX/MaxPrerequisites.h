#pragma once

// MAX VERSION LIST
#define MAX9
//#define MAX2009
//#define MAX2010 // commentare una

#ifdef MAX9
#define ANIMA_MAX_PROGRAM Anima.Max9
#endif
#ifdef MAX2009
#define ANIMA_MAX_PROGRAM Anima.Max2009
#endif
#ifdef MAX2010
#define ANIMA_MAX_PROGRAM Anima.Max2010
#endif
