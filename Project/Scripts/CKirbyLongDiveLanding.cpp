#include "pch.h"
#include "CKirbyLongDiveLanding.h"

CKirbyLongDiveLanding::CKirbyLongDiveLanding()
{
}

CKirbyLongDiveLanding::~CKirbyLongDiveLanding()
{
}


void CKirbyLongDiveLanding::tick()
{
    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::NORMAL: {
        if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
        {
            ChangeState(L"VACUUM1_START");
        }
        else if (PLAYERCTRL->IsGround())
        {
            ChangeState(L"LONGDIVE_BOUND");
        }
    }
    break;
    case AbilityCopyType::FIRE: {
        if (KEY_TAP(KEY_ATK) && PLAYERFSM->IsNearDeformObject())
        {
            ChangeState(L"VACUUM1_START");
        }
        else if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
        {
            if (KEY_PRESSED_ARROW && KEY_TAP(KEY_ATK))
            {
                ChangeState(L"BURNING_PRE");
            }
            else
            {
                ChangeState(L"ATTACK_CHARGE1_START");
            }
        }
        else if (PLAYERCTRL->IsGround())
        {
            ChangeState(L"LONGDIVE_BOUND");
        }
    }
    break;
    case AbilityCopyType::CUTTER: {
        if (KEY_TAP(KEY_ATK) && PLAYERFSM->IsNearDeformObject())
        {
            ChangeState(L"VACUUM1_START");
        }
        else if (KEY_TAP(KEY_ATK))
        {
            if (PLAYERFSM->CanBladeAttack())
            {
                ChangeState(L"ATTACK");
            }
        }
        else if (PLAYERCTRL->IsGround())
        {
            ChangeState(L"LONGDIVE_BOUND");
        }
    }
    break;
    case AbilityCopyType::SWORD: {
        if (KEY_TAP(KEY_ATK) && PLAYERFSM->IsNearDeformObject())
        {
            ChangeState(L"VACUUM1_START");
        }
        else if (PLAYERFSM->GetSlideComboLevel())
        {
            if (PLAYERCTRL->IsGround())
            {
                ChangeState(L"LONGDIVE_BOUND");
            }
            else if ((KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK)) && PLAYERFSM->GetSlideComboLevel() == 1)
            {
                ChangeState(L"JUMP_ATTACK_START");
            }
        }
        else if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
        {
            if (KEY_TAP(KEY_GUARD) || KEY_PRESSED(KEY_GUARD))
            {
                ChangeState(L"ATTACK_AIRGUARD_CHARGE_START");
            }
            else
            {
                ChangeState(L"JUMP_ATTACK_START");
            }
        }
        else if (PLAYERCTRL->IsGround())
        {
            ChangeState(L"LONGDIVE_BOUND");
        }
    }
    break;
    case AbilityCopyType::SLEEP: {
        if (PLAYERCTRL->IsGround())
        {
            ChangeState(L"LONGDIVE_BOUND");
        }
    }
    break;
    }

    
}

void CKirbyLongDiveLanding::Enter()
{
    GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\LongDive.wav", 1, 50.f);

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("LongDiveAttackLanding"), false, false, 2.f);
    
    // Smoke Spawn
    Ptr<CPrefab> LandingSmoke = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbyLandingSmoke.pref");
    CGameObject* LeftSmokeObj = LandingSmoke->Instantiate();
    CGameObject* RightSmokeObj = LandingSmoke->Instantiate();

    Vec3 KirbyFront = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 KirbyRight = PLAYER->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
    Vec3 KirbyPos = PLAYER->Transform()->GetWorldPos();

    LeftSmokeObj->Transform()->SetDirection(-KirbyFront);
    RightSmokeObj->Transform()->SetDirection(-KirbyFront);

    LeftSmokeObj->Transform()->SetWorldPos(KirbyPos - KirbyRight * 4.f + -KirbyFront * 10.f);
    RightSmokeObj->Transform()->SetWorldPos(KirbyPos + KirbyRight * 4.f + -KirbyFront * 10.f);

    GamePlayStatic::SpawnGameObject(LeftSmokeObj, LAYER_EFFECT);
    GamePlayStatic::SpawnGameObject(RightSmokeObj, LAYER_EFFECT);

    PLAYERCTRL->ClearVelocityY();
    PLAYERCTRL->AddVelocity(Vec3(0.f, 7.f, 0.f));

}

void CKirbyLongDiveLanding::Exit()
{
}
