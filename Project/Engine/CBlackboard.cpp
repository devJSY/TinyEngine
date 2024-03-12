#include "pch.h"
#include "CBlackboard.h"

CBlackboard::CBlackboard()
    : m_mapBBData()
{
}

CBlackboard::CBlackboard(const CBlackboard& origin)
    : CEntity(origin)
    , m_mapBBData()
{
}

CBlackboard::~CBlackboard()
{
}

void CBlackboard::AddBlackboardData(const wstring& _strKey, BLACKBOARD_DATA _Type, void* _Data)
{
    map<wstring, tBlackboardData>::iterator iter = m_mapBBData.find(_strKey);

    assert(iter == m_mapBBData.end());

    m_mapBBData.insert(make_pair(_strKey, tBlackboardData{_Type, _Data}));
}

void* CBlackboard::GetBlackboardData(const wstring& _strKey)
{
    map<wstring, tBlackboardData>::iterator iter = m_mapBBData.find(_strKey);

    if (iter == m_mapBBData.end())
        return nullptr;

    return iter->second.pData;
}
