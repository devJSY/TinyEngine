#include "pch.h"
#include "CGrabageCollector.h"

#include "CEntity.h"

CGrabageCollector::CGrabageCollector()
{
}

CGrabageCollector::~CGrabageCollector()
{
    Clear();
}

void CGrabageCollector::tick()
{
    if (100 < m_vecEntity.size())
    {
        Clear();
    }
}

void CGrabageCollector::Clear()
{
    Delete_Vec(m_vecEntity);
}
