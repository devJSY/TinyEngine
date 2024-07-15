#include "pch.h"
#include "CNormalEnemyAttack.h"

#include "CMonsterMgr.h"
#include "CMonsterMoveController.h"

CNormalEnemyAttack::CNormalEnemyAttack()
    : m_pTransform(nullptr)
    , m_pMMC(nullptr)
    , m_pMgr(nullptr)
    , m_fAccTime(0.f)
    , m_fAttackTime(5.f)
{
}

CNormalEnemyAttack::~CNormalEnemyAttack()
{
}

void CNormalEnemyAttack::tick()
{
    m_fAccTime += DT;

    if (m_fAccTime >= m_fAttackTime)
    {
        m_fAccTime = 0.f;
        ChangeState(L"FAILEDATTACK");
        m_pMMC->SetRush(false);
    }
    else
    {
        //ChangeState(L"SUCCESSEDATTACK");
    }
}

void CNormalEnemyAttack::Enter()
{
    // Idle 애니메이션 시작
    if (nullptr == m_pMgr)
    {
        m_pMgr = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager")->GetScript<CMonsterMgr>();
        m_pTransform = GetOwner()->GetComponent<CTransform>();
        m_pMMC = GetOwner()->GetScript<CMonsterMoveController>();
    }
    GetOwner()->GetComponent<CAnimator>()->Play(MONSTERANIM(L"Run"));
    m_pMMC->SetRush(true);
}

void CNormalEnemyAttack::Exit()
{
}