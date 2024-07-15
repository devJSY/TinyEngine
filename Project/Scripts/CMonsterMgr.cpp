#include "pch.h"
#include "CMonsterMgr.h"

CMonsterMgr::CMonsterMgr()
    : CScript(MONSTERMGR)
    , m_pTargetObject(nullptr)
{
}

CMonsterMgr::~CMonsterMgr()
{
}

void CMonsterMgr::begin()
{
    if (FAILED(SetTarget()))
    {
        MessageBox(nullptr, L"CMonsterMgr SetPlayer() 함수가 실패했습니다.", L"CMonsterMgr SetPlayer() Error", MB_OK);
        return;
    }
}

HRESULT CMonsterMgr::SetTarget()
{
    m_pTargetObject = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Player", 4);

    if (m_pTargetObject == nullptr)
    {
        return E_FAIL;
    }

    m_pTargetObjectTr = m_pTargetObject->GetComponent<CTransform>();

    return m_pTargetObjectTr != nullptr ? S_OK : E_FAIL;
}

float CMonsterMgr::CaculateDistance(Vec3 _position)
{
    return (m_pTargetObjectTr->GetLocalPos() - _position).Length();
}

bool CMonsterMgr::IsTrack(Vec3 _position, float _trackDistance)
{
    return CaculateDistance(_position) <= _trackDistance ? true : false;
}