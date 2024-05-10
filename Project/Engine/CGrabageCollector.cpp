#include "pch.h"
#include "CGrabageCollector.h"
#include "CTimeMgr.h"

#include "CEntity.h"

CGrabageCollector::CGrabageCollector()
    : m_vecEntity{}
    , m_AccTime(0.f)
{
}

CGrabageCollector::~CGrabageCollector()
{
    Clear();
}

void CGrabageCollector::tick()
{
    m_AccTime += DT;

    if (m_AccTime > 60.f)
    {
        Clear();
        m_AccTime = 0.f;
    }
}

void CGrabageCollector::Clear()
{
    Delete_Vec(m_vecEntity);
}
