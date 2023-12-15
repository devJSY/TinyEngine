#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
    Delete_Vec(m_vecParents);
}

void CLayer::begin()
{
    for (size_t i = 0; i < m_vecParents.size(); i++)
    {
        m_vecParents[i]->begin();
    }
}

void CLayer::tick()
{
    for (size_t i = 0; i < m_vecParents.size(); i++)
    {
        m_vecParents[i]->tick();
    }
}

void CLayer::finaltick()
{
    for (size_t i = 0; i < m_vecParents.size(); i++)
    {
        m_vecParents[i]->finaltick();
    }
}

void CLayer::render()
{
    for (size_t i = 0; i < m_vecParents.size(); i++)
    {
        m_vecParents[i]->render();
    }
}
