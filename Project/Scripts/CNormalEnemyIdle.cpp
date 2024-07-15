#include "pch.h"
#include "CNormalEnemyIdle.h"

#include "CMonsterMgr.h"
#include "CMonsterMoveController.h"

CNormalEnemyIdle::CNormalEnemyIdle()
    : m_pMgr(nullptr)
    , m_pMMC(nullptr)
    , m_pTransform(nullptr)
{
}

CNormalEnemyIdle::~CNormalEnemyIdle()
{
}

void CNormalEnemyIdle::tick()
{
    bool _IsTrack = m_pMgr->IsTrack(m_pTransform->GetLocalPos(), m_pMMC->GetTrackDist());

    if (_IsTrack)
    {
        ChangeState(L"TRACK");
        m_pMMC->SetTrack(true);
    }
}

void CNormalEnemyIdle::Enter()
{
    // Idle 애니메이션 시작
    if (nullptr == m_pMgr)
    {
        m_pMgr = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager")->GetScript<CMonsterMgr>();
        m_pTransform = GetOwner()->GetComponent<CTransform>();
        m_pMMC = GetOwner()->GetScript<CMonsterMoveController>();
        //GetOwner()->GetComponent<CAnimator>()->Play(L"");
    }
}

void CNormalEnemyIdle::Exit()
{
}