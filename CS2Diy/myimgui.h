#pragma once
#include "ImGui/imconfig.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imstb_rectpack.h"
#include "ImGui/imstb_textedit.h"
#include "ImGui/imstb_truetype.h"
#include "tool.h"
#include "esp.h"

#include <iostream>
#include <Windows.h>
#include <thread>
#include <d3d11.h>
#include <dwmapi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")

namespace myimgui
{
    typedef void (*myFun)(void);

    struct windowinfo
    {
        HWND hwnd = nullptr;
        LPCSTR ClassName = nullptr;
        LPCSTR TitleName = nullptr;
        ImVec2 pos = { 0, 0 };
        ImVec2 size = { 0, 0 };
    };

    inline windowinfo mywindow;
    inline windowinfo gamewindow;

    inline static ID3D11Device* g_pd3dDevice = nullptr;
    inline static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    inline static IDXGISwapChain* g_pSwapChain = nullptr;
    inline static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
    inline static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

    int CreateWindow_Violet(LPCSTR class_name, LPCSTR title_name, myimgui::myFun Fun, const char* Fontsname, float Fonts_size, bool Synclnterval, int Menustyle);
    void UpdateWindow();
    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}
