#include "pch.h"
#include "CW9_Boss_CinematicScript.h"
#include "CLifeScript.h"

CW9_Boss_CinematicScript::CW9_Boss_CinematicScript()
{
}

CW9_Boss_CinematicScript::~CW9_Boss_CinematicScript()
{
}

void CW9_Boss_CinematicScript::begin()
{
    CCinematicScript::begin();
}

void CW9_Boss_CinematicScript::tick()
{
    CCinematicScript::tick();
}

void CW9_Boss_CinematicScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    if (!m_bTrigger || L"Player" != _OtherCollider->GetOwner()->GetName())
        return;

    CCinematicScript::OnTriggerEnter(_OtherCollider);

    CGameObject* pLife = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"W09_Boss_Natalie");
    if (nullptr != pLife)
    {
        CLifeScript* pLifeScript = pLife->GetScript<CLifeScript>();
        if (nullptr != pLifeScript)
        {
            pLifeScript->ChangeState(LIFE_STATE::Intro);
        }
    }
}
