#include "pch.h"
#include "CEngine.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CTestLevel.h"
#include "CDevice.h"
#include "CEditorMgr.h"
#include "CEditor.h"

CLevelMgr::CLevelMgr()
    : m_CurLevel(nullptr)
    , m_CamObj(nullptr)
    , m_SelectedObj(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
    if (nullptr != m_CurLevel)
    {
        delete m_CurLevel;
        m_CurLevel = nullptr;
    }
}

void CLevelMgr::init()
{
    m_CurLevel = new CTestLevel;
    m_CurLevel->begin();
    m_CurLevel->SetName(L"Test Level 1");
}

void CLevelMgr::tick()
{
    if (nullptr == m_CurLevel)
        return;

    // 이전 프레임에 등록된 오브젝트들 clear
    m_CurLevel->clear();

    m_CurLevel->tick();
    m_CurLevel->finaltick();
}

void CLevelMgr::render()
{
    if (nullptr == m_CurLevel)
        return;

    m_CurLevel->render();
}
