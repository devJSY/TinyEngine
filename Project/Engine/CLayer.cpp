#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
    Delete_Vec(m_vecParent);
}

void CLayer::begin()
{
    for (size_t i = 0; i < m_vecParent.size(); i++)
    {
        m_vecParent[i]->begin();
    }
}

void CLayer::tick()
{
    for (size_t i = 0; i < m_vecParent.size(); i++)
    {
        m_vecParent[i]->tick();
    }
}

void CLayer::finaltick()
{
    for (size_t i = 0; i < m_vecParent.size(); i++)
    {
        m_vecParent[i]->finaltick();
    }
}

void CLayer::render()
{
    for (size_t i = 0; i < m_vecParent.size(); i++)
    {
        m_vecParent[i]->render();
    }
}
