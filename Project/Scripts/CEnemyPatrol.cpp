#include "pch.h"
#include "CEnemyPatrol.h"

#include "CMonsterMgr.h"
#include "CMonsterMoveController.h"

CEnemyPatrol::CEnemyPatrol()
    : m_pMgr(nullptr)
    , m_pTransform(nullptr)
    , m_pMMC(nullptr)
{
}

CEnemyPatrol::~CEnemyPatrol()
{
}

void CEnemyPatrol::tick()
{
    bool _IsTrack = m_pMgr->IsTrack(m_pTransform->GetLocalPos(), m_pMMC->GetTrackDist());

    if (!_IsTrack)
    {
        ChangeState(L"IDLE");
    }
}

void CEnemyPatrol::Enter()
{
    if (nullptr == m_pMgr)
    {
        m_pMgr = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager")->GetScript<CMonsterMgr>();
        m_pTransform = GetOwner()->GetComponent<CTransform>();
        m_pMMC = GetOwner()->GetScript<CMonsterMoveController>();
    }
}

void CEnemyPatrol::Exit()
{
    m_pMMC->SetTrack(false);
}