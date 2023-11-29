// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <stdio.h>
#include <CommCtrl.h>

// Visual Studio is rather dumb at times..
// -> moved to project settings
/*#define _USE_MATH_DEFINES*/ 
#include <cmath>

#include <malloc.h>
#include <memory.h>

extern FILE* pConsole;

#ifdef DEBUG_CONSOLE
#define LOG(s, ...) if ( pConsole != nullptr) fprintf(pConsole, s, __VA_ARGS__);
#endif //

