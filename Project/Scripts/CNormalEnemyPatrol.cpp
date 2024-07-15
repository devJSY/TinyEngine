#include "pch.h"
#include "CNormalEnemyPatrol.h"

#include "CMonsterMgr.h"
#include "CMonsterMoveController.h"

CNormalEnemyPatrol::CNormalEnemyPatrol()
    : m_pMgr(nullptr)
    , m_pTransform(nullptr)
    , m_pMMC(nullptr)
{
}

CNormalEnemyPatrol::~CNormalEnemyPatrol()
{
}

void CNormalEnemyPatrol::tick()
{
    bool _IsTrack = m_pMgr->IsTrack(m_pTransform->GetLocalPos(), m_pMMC->GetTrackDist());

    if (!_IsTrack)
    {
        ChangeState(L"IDLE");
    }
}

void CNormalEnemyPatrol::Enter()
{
    if (nullptr == m_pMgr)
    {
        m_pMgr = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager")->GetScript<CMonsterMgr>();
        m_pTransform = GetOwner()->GetComponent<CTransform>();
        m_pMMC = GetOwner()->GetScript<CMonsterMoveController>();
    }
}

void CNormalEnemyPatrol::Exit()
{
    m_pMMC->SetTrack(false);
}