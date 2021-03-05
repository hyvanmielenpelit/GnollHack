// dllmain.cpp : Defines the entry point for the DLL application.
#include "win32api.h" /* for GetModuleFileName */
#include "pch.h"
#include "direct.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

DLL int DoSomeCalc2()
{
    HINSTANCE hinst = GetModuleHandle(NULL);
    char buf[256];
    int length = 0;
    if(getcwd(buf, 256))
        length = strlen(buf);
    return length;
}
