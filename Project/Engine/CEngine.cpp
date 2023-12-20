#include "pch.h"
#include "CEngine.h"
#include "CDevice.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CEditorMgr.h"
#include "CTaskMgr.h"

CEngine::CEngine()
    : m_hMainWnd(nullptr)
    , m_vResolution()
{
}

CEngine::~CEngine()
{
}

int CEngine::init(HWND _hWnd, Vec2 _vResolution)
{
    m_hMainWnd = _hWnd;
    m_vResolution = _vResolution;

    RECT rt = {0, 0, (int)m_vResolution.x, (int)m_vResolution.y};
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, SetMenu(m_hMainWnd, nullptr)); // 메뉴바 Off
    SetWindowPos(m_hMainWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

    if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
    {
        MessageBox(nullptr, L"Device 초기화 실패", L"초기화 실패", MB_OK);
        return E_FAIL;
    }

    // Manager 초기화
    CPathMgr::init();
    CTimeMgr::GetInst()->init();
    CKeyMgr::GetInst()->init();
    CAssetMgr::GetInst()->init();
    CLevelMgr::GetInst()->init();
    CEditorMgr::GetInst()->init();

    return S_OK;
}

void CEngine::progress()
{
    // Manager Update
    CTimeMgr::GetInst()->tick();
    CKeyMgr::GetInst()->tick();

    // Clear
    float ClearColor[4] = {0.3f, 0.8f, 0.3f, 1.f};
    CDevice::GetInst()->ClearRenderTarget(ClearColor);

    // Level Update
    CEditorMgr::GetInst()->tick();
    CLevelMgr::GetInst()->tick();

    // Level Render
    CEditorMgr::GetInst()->render();
    CLevelMgr::GetInst()->render();

    // Present
    CDevice::GetInst()->Present();

    // task Execute
    CTaskMgr::GetInst()->tick();
}
