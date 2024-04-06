#include "pch.h"
#include "CHallofEternity_Floor1_CinematicScript.h"
#include "CEvilPumpkinScript.h"

CHallofEternity_Floor1_CinematicScript::CHallofEternity_Floor1_CinematicScript()
    : CCinematicScript(HALLOFETERNITY_FLOOR1_CINEMATICSCRIPT)
{
}

CHallofEternity_Floor1_CinematicScript::~CHallofEternity_Floor1_CinematicScript()
{
}

void CHallofEternity_Floor1_CinematicScript::begin()
{
    CCinematicScript::begin();
}

void CHallofEternity_Floor1_CinematicScript::tick()
{
    CCinematicScript::tick();

    // 테스트용 코드
    if (KEY_TAP(KEY::T))
        CGameManagerScript::GetInset()->GetPlayer()->Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(-2000.f, 0.f, 0.f));
}

void CHallofEternity_Floor1_CinematicScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    if (!m_bTrigger || L"Player" != _OtherCollider->GetOwner()->GetName())
        return;

    CCinematicScript::OnTriggerEnter(_OtherCollider);

    CGameObject* pEvilPumpkin = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"EvilPumpkin");
    if (nullptr != pEvilPumpkin)
    {
        CEvilPumpkinScript* pEvilPumpkinScript = pEvilPumpkin->GetScript<CEvilPumpkinScript>();
        if (nullptr != pEvilPumpkinScript)
        {
            pEvilPumpkinScript->ChangeState(EVILPUMPKINSCRIPT_STATE::Intro);
        }
    }
}

void CHallofEternity_Floor1_CinematicScript::SaveToLevelFile(FILE* _File)
{
    CCinematicScript::SaveToLevelFile(_File);
}

void CHallofEternity_Floor1_CinematicScript::LoadFromLevelFile(FILE* _File)
{
    CCinematicScript::LoadFromLevelFile(_File);
}
