#include "pch.h"
#include "CLevel.h"

#include "CLayer.h"

CLevel::CLevel()
    : m_arrLayer{}
{
    for (UINT i = 0; i < LAYER_MAX; ++i)
    {
        m_arrLayer[i] = new CLayer;
    }
}

CLevel::~CLevel()
{
    Delete_Array(m_arrLayer);
}

void CLevel::begin()
{
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        m_arrLayer[i]->begin();
    }
}

void CLevel::tick()
{
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        m_arrLayer[i]->tick();
    }
}

void CLevel::finaltick()
{
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        m_arrLayer[i]->finaltick();
    }
}

void CLevel::render()
{
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        m_arrLayer[i]->render();
    }
}

void CLevel::AddObject(CGameObject* _Object, int _LayerIdx)
{
    m_arrLayer[_LayerIdx]->AddObject(_Object);
}
