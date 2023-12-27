#include "pch.h"
#include "CGrabageCollector.h"

#include "CEntity.h"

CGrabageCollector::CGrabageCollector()
{
}

CGrabageCollector::~CGrabageCollector()
{
    Delete_Vec(m_vecEntity);
}

void CGrabageCollector::tick()
{
    if (100 < m_vecEntity.size())
    {
        Delete_Vec(m_vecEntity);
    }
}
