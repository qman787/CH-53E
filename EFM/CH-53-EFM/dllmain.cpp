// dllmain.cpp : Defines the entry point for the DLL application.

#define WIN32_LEAN_AND_MEAN

#include "stdafx.h"
#include "pch.h"

BOOL boConsoleAllocated = false;
FILE* pConsole = nullptr;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
#ifdef DEBUG_CONSOLE
        boConsoleAllocated = AllocConsole();
        if (boConsoleAllocated)
        {
            freopen_s(&pConsole, "CONOUT$", "w", stdout);
            SetConsoleTitleA("CH-53 EFM Debug Console");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_BLUE);
            LOG("DLL_PROCESS_ATTACH\n");
        }
#endif //DEBUG_CONSOLE
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

