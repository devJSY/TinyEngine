#include "pch.h"
#include "CEngine.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CDevice.h"
#include "CEditorMgr.h"
#include "CEditor.h"
#include "CLayer.h"

#include "CTestLevel.h"
#include "CTestLevel2.h"

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

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
    if (nullptr != m_CurLevel)
    {
        delete m_CurLevel;
        m_CurLevel = nullptr;
    }

    m_CurLevel = _NextLevel;
}

void CLevelMgr::init()
{
    /*m_CurLevel = new CTestLevel;
    m_CurLevel->SetName(L"Test Level 1");

    for (int i = 0; i < LAYER_MAX; i++)
    {
        wstring Name = L"Layer ";
        Name += std::to_wstring(i);
        m_CurLevel->GetLayer(i)->SetName(Name);
    }

    m_CurLevel->GetLayer(0)->SetName(L"Default");
    m_CurLevel->GetLayer(1)->SetName(L"Camera");
    m_CurLevel->GetLayer(2)->SetName(L"Light");
    m_CurLevel->GetLayer(3)->SetName(L"Mesh");
    m_CurLevel->GetLayer(4)->SetName(L"Model");
    m_CurLevel->GetLayer(14)->SetName(L"SkyBox");
    m_CurLevel->GetLayer(15)->SetName(L"UI");*/

    m_CurLevel = new CTestLevel2;
    m_CurLevel->SetName(L"Test Level 2");

    for (int i = 0; i < LAYER_MAX; i++)
    {
        wstring Name = L"Layer ";
        Name += std::to_wstring(i);
        m_CurLevel->GetLayer(i)->SetName(Name);
    }

    m_CurLevel->GetLayer(0)->SetName(L"Default");
    m_CurLevel->GetLayer(1)->SetName(L"Camera");
    m_CurLevel->GetLayer(2)->SetName(L"Player");
    m_CurLevel->GetLayer(3)->SetName(L"Monster");
    m_CurLevel->GetLayer(4)->SetName(L"Light");
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
