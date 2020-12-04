#pragma once
#include "stdafx.h"
#include "AMain.hpp"
#include "DImGuiManager.hpp"

ID3D11RenderTargetView* rendertarget;
ID3D11DeviceContext* context;
ID3D11Device* device;
HRESULT(*HK_PresentOriginal)(IDXGISwapChain* swapchain, UINT sync, UINT flags) = nullptr;
int X, Y;
HWND hwnd;
HRESULT HK_PresentHook(IDXGISwapChain* swapchain, UINT sync, UINT flags)
{
    if (!device)
    {
        ID3D11Texture2D* renderTarget = 0;
        ID3D11Texture2D* backBuffer = 0;
        D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };

        swapchain->GetDevice(__uuidof(device), (PVOID*)&device);
        device->GetImmediateContext(&context);

        swapchain->GetBuffer(0, __uuidof(renderTarget), (PVOID*)&renderTarget);
        device->CreateRenderTargetView(renderTarget, nullptr, &rendertarget);
        renderTarget->Release();

        swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)&backBuffer);
        backBuffer->GetDesc(&backBufferDesc);

        X = backBufferDesc.Width;
        Y = backBufferDesc.Height;

        backBuffer->Release();

        if (!hwnd) hwnd = GetForegroundWindow();

        ImGui_ImplDX11_Init(hwnd, device, context);
        ImGui_ImplDX11_CreateDeviceObjects();
    }

    context->OMSetRenderTargets(1, &rendertarget, nullptr);

    DImGuiManager::Initialize();

    AMain::Draw();

    ImGui::GetOverlayDrawList()->PushClipRectFullScreen();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::Render();

    return HK_PresentOriginal(swapchain, sync, flags);
}