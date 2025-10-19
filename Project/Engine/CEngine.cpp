#include "pch.h"
#include "CEngine.h"
#include "CDevice.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CGrabageCollector.h"
#include "CRenderMgr.h"
#include "CPhysics2DMgr.h"
#include "CFontMgr.h"
#include "CPhysicsMgr.h"
#include "CInstancingBuffer.h"
#include "CSoundMgr.h"

#include "CSound.h"

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
    SetMenu(m_hMainWnd, nullptr);
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false); // 메뉴바 Off
    SetWindowPos(m_hMainWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

    if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
    {
        MessageBox(nullptr, L"Device 초기화 실패", L"초기화 실패", MB_OK);
        return E_FAIL;
    }

    CLevelMgr::GetInst()->init();
    CPathMgr::init();
    CTimeMgr::GetInst()->init();
    CKeyMgr::GetInst()->init();
    CAssetMgr::GetInst()->init();
    CRenderMgr::GetInst()->init();
    CFontMgr::GetInst()->init();
    CInstancingBuffer::GetInst()->init();

    return S_OK;
}

void CEngine::tick()
{
    CTimeMgr::GetInst()->tick();
    CKeyMgr::GetInst()->tick();

    CLevelMgr::GetInst()->tick();

    CPhysics2DMgr::GetInst()->tick();
    CPhysicsMgr::GetInst()->tick();

    CSoundMgr::GetInst()->tick();
    CSound::g_pFMOD->update();
}

void CEngine::render()
{ 
    CRenderMgr::GetInst()->render();
}

void CEngine::present()
{
    CDevice::GetInst()->Present();
}

void CEngine::clear()
{
    CGrabageCollector::GetInst()->tick();

    CTaskMgr::GetInst()->tick();

    CKeyMgr::GetInst()->clear();
}
