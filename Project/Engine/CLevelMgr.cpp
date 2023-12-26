#include "pch.h"
#include "CEngine.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CTestLevel.h"

CLevelMgr::CLevelMgr()
    : m_CurLevel(nullptr)
    , m_CamObj(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
    if (nullptr != m_CurLevel)
        delete m_CurLevel;
}

void CLevelMgr::init()
{
     m_CurLevel = new CTestLevel;
     m_CurLevel->begin();
     m_CurLevel->SetName(L"Test Level 1");
}

void CLevelMgr::tick()
{
    // 윈도우 사이즈 체크
    RECT rect;
    if (GetClientRect(CEngine::GetInst()->GetMainWind(), &rect))
    {
        Vec2 Resolution = CEngine::GetInst()->GetResolution();
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        if (Resolution.x != width || Resolution.y != height)
        {
            FTask task;
            task.Type = TASK_TYPE::WINDOW_RESIZE;
            task.Param_1 = (INT_PTR)width;
            task.Param_2 = (INT_PTR)height;

            CTaskMgr::GetInst()->AddTask(task);
        }
    }

    if (nullptr == m_CurLevel)
        return;

    m_CurLevel->tick();
    m_CurLevel->finaltick();
}

void CLevelMgr::render()
{
    if (nullptr == m_CurLevel)
        return;

    m_CurLevel->render();
}
