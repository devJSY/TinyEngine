#include "pch.h"
#include "CEnemyIdle.h"

#include "CMonsterMgr.h"
#include "CMonsterMoveController.h"

CEnemyIdle::CEnemyIdle()
    : m_pMgr(nullptr)
    , m_pMMC(nullptr)
    , m_pTransform(nullptr)
{
}

CEnemyIdle::~CEnemyIdle()
{
}

void CEnemyIdle::tick()
{
    bool _IsTrack = m_pMgr->IsTrack(m_pTransform->GetLocalPos(), m_pMMC->GetTrackDist());

    if (_IsTrack)
    {
        ChangeState(L"FIND");
        m_pMMC->SetTrack(true);
    }
}

void CEnemyIdle::Enter()
{
    // Idle 애니메이션 시작
    if (nullptr == m_pMgr)
    {
        GetOwner()->GetComponent<CAnimator>()->Play(MONSTERANIM(L"Wait"));
        m_pMgr = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager")->GetScript<CMonsterMgr>();
        m_pTransform = GetOwner()->GetComponent<CTransform>();
        m_pMMC = GetOwner()->GetScript<CMonsterMoveController>();
    }
}

void CEnemyIdle::Exit()
{
}