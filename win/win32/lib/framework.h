#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include "tradstdc.h"

#define DLL __declspec(dllexport)

/* Test functions*/
DLL int DoSomeCalc2();
