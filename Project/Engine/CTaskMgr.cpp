#include "pch.h"

#include "CTaskMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CGameObject.h"
#include "CComponent.h"

#include "CEngine.h"
#include "CDevice.h"
#include "CEditorMgr.h"
#include "CRenderMgr.h"

CTaskMgr::CTaskMgr()
{
}

CTaskMgr::~CTaskMgr()
{
}

void CTaskMgr::tick()
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
            GamePlayStatic::WindowResize(width, height);
        }
    }

    for (size_t i = 0; i < m_vecTask.size(); ++i)
    {
        switch (m_vecTask[i].Type)
        {
        case TASK_TYPE::CREATE_OBJECT:
            {
                int LayerIdx = (int)m_vecTask[i].Param_1;
                CGameObject* Object = (CGameObject*)m_vecTask[i].Param_2;

                CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
                pCurLevel->AddObject(Object, LayerIdx, true);

                /*if (LEVEL_STATE::PLAY == pCurLevel->GetState())
                {
                    Object->begin();
                }*/
            }
            break;
        case TASK_TYPE::DELETE_OBJECT:
            {
                CGameObject* pDeadObj = (CGameObject*)m_vecTask[i].Param_1;

                list<CGameObject*> queue;
                queue.push_back(pDeadObj);

                // 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 Dead 처리
                while (!queue.empty())
                {
                    CGameObject* pObject = queue.front();
                    queue.pop_front();

                    pObject->m_bDead = true;

                    for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
                    {
                        queue.push_back(pObject->m_vecChild[i]);
                    }
                }
            }
            break;
        case TASK_TYPE::LEVEL_CHANGE:
            {
            }
            break;
        case TASK_TYPE::ADD_CHILD:
            break;
        case TASK_TYPE::DISCONNECT_PARENT:
            break;
        case TASK_TYPE::WINDOW_RESIZE:
            {
                UINT width = (UINT)m_vecTask[i].Param_1;
                UINT height = (UINT)m_vecTask[i].Param_2;
                if (width <= 0 || height <= 0)
                    break;

                Vec2 resolution = Vec2(width, height);
                CEngine::GetInst()->SetResolution(resolution);
                CDevice::GetInst()->Resize(resolution);
                CRenderMgr::GetInst()->Resize(resolution);
            }
            break;
        }
    }

    m_vecTask.clear();
}