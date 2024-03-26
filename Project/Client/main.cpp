#include "pch.h"
#include "Client.h"
#include "framework.h"

#include <Engine\\CEngine.h>
#include <Engine\\CKeyMgr.h>

// Engine
#ifdef _DEBUG
    #pragma comment(lib, "Engine\\Engine_d.lib")
#else
    #pragma comment(lib, "Engine\\Engine.lib")
#endif

// Scripts
#ifdef _DEBUG
    #pragma comment(lib, "Scripts\\Scripts_d.lib")
#else
    #pragma comment(lib, "Scripts\\Scripts.lib")
#endif

#include "CCreateTestLevel.h"
#include "CCreatePBRLevel.h"

#include <Engine\\CLevelSaveLoad.h>

// 메모리 누수 체크
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

// ImGui
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;
HWND hWnd;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    // 메모리 누수 체크
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(14015);

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    if (FAILED(CEngine::GetInst()->init(hWnd, Vec2(1280, 768))))
    {
        MessageBox(nullptr, L"CEngine 초기화 실패", L"초기화 실패", MB_OK);
        return 0;
    }

    // CCreateTestLevel::Init();
    // CCreateTestLevel::CreateTestLevel();
    // CCreatePBRLevel::Init();
    // CCreatePBRLevel::CreatePBRLevel();

    // GamePlayStatic::ChangeLevel(CLevelSaveLoad::LoadLevel(L"Home Level.tLevel"), LEVEL_STATE::PLAY);
    GamePlayStatic::ChangeLevel(CLevelSaveLoad::LoadLevel(L"Test Level.tLevel"), LEVEL_STATE::STOP);

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            CEngine::GetInst()->progress();
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LOGO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName = L"TinyEngine";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_LOGO));

    return RegisterClassExW(&wcex);
}

#include <dwmapi.h> // TitleBar Drak Mode

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    hWnd =
        CreateWindowW(L"TinyEngine", L"Tiny Engine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    // TitleBar Drak Mode
    BOOL USE_DARK_MODE = true;
    DwmSetWindowAttribute(hWnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &USE_DARK_MODE, sizeof(USE_DARK_MODE));

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_MOUSEWHEEL: {
        // ↑ : 120 , ↓ : -120
        short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        CKeyMgr::GetInst()->SetMouseWheel(zDelta);
    }
    break;
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
