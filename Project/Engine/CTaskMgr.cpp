#include "pch.h"
#include "CTaskMgr.h"

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
            }
            break;
        }
    }

    m_vecTask.clear();
}
