/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.1.0 gnollhackwin.c */
/* Copyright (c) Janne Gustafsson, 2024. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "gnollhackwin.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

/* Moves from over to atomically; retries because a scanner can hold the target for a moment.
   Returns 0 on success, otherwise the Windows error code. */
int
gnhwin_move_file_replacing(const char *from, const char *to)
{
    int i;

    for (i = 0; ; i++)
    {
        DWORD err;

        if (MoveFileExA(from, to, MOVEFILE_REPLACE_EXISTING))
            return 0;
        err = GetLastError();
        if ((err != ERROR_ACCESS_DENIED && err != ERROR_SHARING_VIOLATION) || i >= 4)
            return (int) err;
        Sleep(20);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule,
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
