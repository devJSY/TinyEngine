#include "pch.h"
#include "CTaskMgr.h"

#include "CEngine.h"
#include "CDevice.h"

CTaskMgr::CTaskMgr()
{
}

CTaskMgr::~CTaskMgr()
{
}

void CTaskMgr::tick()
{
    for (size_t i = 0; i < m_vecTask.size(); ++i)
    {
        switch (m_vecTask[i].Type)
        {
        case CREATE_OBJECT:
            {
            }
            break;
        case DELETE_OBJECT:
            {
            }
            break;
        case LEVEL_CHANGE:
            {
            }
            break;
        case WINDOW_RESIZE:
            {
                UINT width = (UINT)m_vecTask[i].Param_1;
                UINT height = (UINT)m_vecTask[i].Param_2;
                CEngine::GetInst()->SetResolution(Vec2(width, height));

                std::cout << "Window Size Changed !!" << std::endl;
            }
            break;
        }
    }

    m_vecTask.clear();
}
