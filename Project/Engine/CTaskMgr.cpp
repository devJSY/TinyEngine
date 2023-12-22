#include "pch.h"
#include "CTaskMgr.h"

#include "CEngine.h"
#include "CDevice.h"
#include "CEditorMgr.h"
#include "CKeyMgr.h"

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
                if (width <= 0 || height <= 0) 
                    break;

                Vec2 resolution = Vec2(width, height);
                CEngine::GetInst()->SetResolution(resolution);
                CDevice::GetInst()->Resize(resolution);
                CEditorMgr::GetInst()->Resize();

                std::cout << "Window Size Changed !!" << std::endl;
            }
            break;
        }
    }

    m_vecTask.clear();
}
