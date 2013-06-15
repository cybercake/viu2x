#ifndef OS_H
#define OS_H

#include "CompilationSettings.h"

#define UNICODE
#define _UNICODE

#ifdef VIU2X_WINDOWS
#include <windows.h>
#include <winnt.h>
#include <Winnls.h>
#endif

#ifdef VIU2X_LINUX
The viu2x library is not implemented for Linux!
#endif

#endif // OS_H
