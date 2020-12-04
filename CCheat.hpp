#pragma once
#include "FMemoryManager.hpp"
#include "BHooks.hpp"
namespace CCheat
{
    void hook(__int64 addr, __int64 func, __int64* orig)
    {
        static uintptr_t hook_addr;
        if (!hook_addr)
            hook_addr = sigscan("GameOverlayRenderer64.dll", "48 ? ? ? ? 57 48 83 EC 30 33 C0");

        auto hook = ((__int64(__fastcall*)(__int64 addr, __int64 func, __int64* orig, __int64 smthng))(hook_addr));
        hook((__int64)addr, (__int64)func, orig, (__int64)1);
    }
	CBoolean Initialize()
	{
        if (!GetModuleHandleA("GameOverlayRenderer64.dll"))
        {
            MessageBoxA(0, "SteamOverlayRenderer64.dll Not Loaded!", "Fatal Error", MB_ICONERROR);
            exit(0);
        }

        uintptr_t Steam_DXGI_PresentScene = sigscan("GameOverlayRenderer64.dll", "48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC 20 41 8B E8");

        if (Steam_DXGI_PresentScene)
            hook(Steam_DXGI_PresentScene, (__int64)HK_PresentHook, (__int64*)&HK_PresentOriginal);
        else
        {
            MessageBox(GetForegroundWindow(), L"Fail", L"CCheat.hpp - Line 29", 0);
            return 0;
        }
	}
}