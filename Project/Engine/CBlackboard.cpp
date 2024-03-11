#include "pch.h"
#include "CBlackboard.h"

CBlackboard::CBlackboard()
    : m_mapBBData()
{
}

CBlackboard::CBlackboard(const CBlackboard& _board)
    : CEntity(_board)
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
