#include "myimgui.h"
#include <Windows.h>
#include <thread>

int myimgui::CreateWindow_Violet(LPCSTR className, LPCSTR titleName, myimgui::myFun fun, const char* fontsName, float fontSize, bool syncInterval, int menuStyle)
{
    gamewindow.ClassName = className;
    gamewindow.TitleName = titleName;

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Violet", nullptr };
    ::RegisterClassExW(&wc);
    mywindow.hwnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        wc.lpszClassName, L"CS2 External SDK", WS_POPUP, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    if (!myimgui::CreateDeviceD3D(mywindow.hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(mywindow.hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(mywindow.hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     

    if (menuStyle == 0) { ImGui::StyleColorsDark(); }
    if (menuStyle == 1) { ImGui::StyleColorsLight(); }
    
    ImGui_ImplWin32_Init(mywindow.hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Optimized font loading
    io.Fonts->AddFontFromFileTTF(fontsName, fontSize, nullptr, io.Fonts->GetGlyphRangesChineseFull());

    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);
    bool done = false;
    
    // Start background processing thread
    std::thread backgroundThread(CheatThread);

    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            exit(0);

        UpdateWindow();

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        
        fun(); 
        
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        
        if (syncInterval)
        {
            g_pSwapChain->Present(1, 0);
        }
        else
        {
            g_pSwapChain->Present(0, 0);
        }
    }

    backgroundThread.detach(); // Allow thread to exit with process

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(mywindow.hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    return 0;
}

void myimgui::UpdateWindow()
{
    POINT point{};
    RECT rect{};

    gamewindow.hwnd = FindWindowA(gamewindow.ClassName, gamewindow.TitleName);

    if (gamewindow.hwnd) {
        GetClientRect(gamewindow.hwnd, &rect);
        ClientToScreen(gamewindow.hwnd, &point);

        mywindow.pos = gamewindow.pos = ImVec2((float)point.x, (float)point.y);
        mywindow.size = gamewindow.size = ImVec2((float)rect.right, (float)rect.bottom);
        
        // Update global cheat resolution
        cheat::screenWidth = mywindow.size.x;
        cheat::screenHeight = mywindow.size.y;

        SetWindowPos(mywindow.hwnd, HWND_TOPMOST, (int)mywindow.pos.x, (int)mywindow.pos.y, (int)mywindow.size.x, (int)mywindow.size.y, SWP_SHOWWINDOW);
    }

    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(mywindow.hwnd, &mousePos);
    ImGui::GetIO().MousePos.x = (float)mousePos.x;
    ImGui::GetIO().MousePos.y = (float)mousePos.y;

    if (ImGui::GetIO().WantCaptureMouse)
    {
        SetWindowLong(mywindow.hwnd, GWL_EXSTYLE, GetWindowLong(mywindow.hwnd, GWL_EXSTYLE) & (~WS_EX_LAYERED));
    }
    else
    {
        SetWindowLong(mywindow.hwnd, GWL_EXSTYLE, GetWindowLong(mywindow.hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    }
}

bool myimgui::CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void myimgui::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void myimgui::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void myimgui::CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI myimgui::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_CREATE:
    {
        MARGINS Margin = { -1 };
        DwmExtendFrameIntoClientArea(hWnd, &Margin);
        break;
    }
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
