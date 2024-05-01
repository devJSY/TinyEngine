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
#include "CGrabageCollector.h"
#include "CRenderMgr.h"
#include "CPhysics2DMgr.h"
#include "CFontMgr.h"
#include "CPhysicsMgr.h"

#include "CSound.h"

CEngine::CEngine()
    : m_hMainWnd(nullptr)
    , m_vResolution()
{
}

CEngine::~CEngine()
{
    if (nullptr != CSound::g_pFMOD)
    {
        CSound::g_pFMOD->release();
        CSound::g_pFMOD = nullptr;
    }
}

int CEngine::init(HWND _hWnd, Vec2 _vResolution)
{
    m_hMainWnd = _hWnd;
    m_vResolution = _vResolution;

    RECT rt = {0, 0, (int)m_vResolution.x, (int)m_vResolution.y};
    SetMenu(m_hMainWnd, nullptr);
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false); // 메뉴바 Off
    SetWindowPos(m_hMainWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

    if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
    {
        MessageBox(nullptr, L"Device 초기화 실패", L"초기화 실패", MB_OK);
        return E_FAIL;
    }

    // Manager 초기화
    CLevelMgr::GetInst()->init();
    CPathMgr::init();
    CTimeMgr::GetInst()->init();
    CKeyMgr::GetInst()->init();
    CAssetMgr::GetInst()->init();
    CRenderMgr::GetInst()->init();
    CEditorMgr::GetInst()->init();
    CFontMgr::GetInst()->init();

    return S_OK;
}

void CEngine::progress()
{
    // ===========================
    // tick
    // ===========================
    {
        // Manager Update
        CTimeMgr::GetInst()->tick();
        CKeyMgr::GetInst()->tick();

        // FMOD Update
        CSound::g_pFMOD->update();

        // Level Update
        CEditorMgr::GetInst()->tick();
        CLevelMgr::GetInst()->tick();

        // Physics Update
        CPhysics2DMgr::GetInst()->tick();
        CPhysicsMgr::GetInst()->tick();
    }

    // ===========================
    // Rendering
    // ===========================
    {
        // Level Render
        CRenderMgr::GetInst()->render();
        CEditorMgr::GetInst()->render(); // Level 렌더링 이후 호출

        // Present
        CDevice::GetInst()->Present();
    }

    // ===========================
    // Clear
    // ===========================
    {
        // Grabage Collector
        CGrabageCollector::GetInst()->tick();

        // Task Execute
        CTaskMgr::GetInst()->tick();

        CKeyMgr::GetInst()->clear();
    }
}
