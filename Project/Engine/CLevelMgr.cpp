#include "pch.h"
#include "CEngine.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CTestLevel.h"
#include "CDevice.h"
#include "CEditorMgr.h"
#include "CEditor.h"
#include "CLayer.h"

CLevelMgr::CLevelMgr()
    : m_CurLevel(nullptr)
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
    m_CurLevel->SetName(L"Test Level 1");
    m_CurLevel->GetLayer(0)->SetName(L"Default");
    m_CurLevel->GetLayer(1)->SetName(L"Camera");
    m_CurLevel->GetLayer(2)->SetName(L"Light");
    m_CurLevel->GetLayer(3)->SetName(L"Mesh");
    m_CurLevel->GetLayer(4)->SetName(L"Model");
    m_CurLevel->GetLayer(14)->SetName(L"SkyBox");
    m_CurLevel->GetLayer(15)->SetName(L"UI");

    m_CurLevel->begin();
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
