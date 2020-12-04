
/* 
    Project: Steam-Hook-Render-PoC
    Author: furiosdestruct
    File: dllmain.cpp
*/

#include "stdafx.h"
#include "CCheat.hpp"

bool __stdcall DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH)
    {
        CCheat::Initialize();
    }
    return false;
}