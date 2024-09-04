#include "pch.h"
#include "CKirbyRun.h"

#define RUN_SPEED 10.f

CKirbyRun::CKirbyRun()
{
}

CKirbyRun::~CKirbyRun()
{
}

void CKirbyRun::tick()
{
    SpawnSmoke();

    PLAY_CURSTATE(Run)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
        switch (PLAYERFSM->GetCurObjectIdx())
        {
        case ObjectCopyType::CONE: {
            if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                ChangeState(L"ATTACK_START");
            }
            else if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
            {
                ChangeState(L"DROP_OBJECT_START");
            }
            else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
            {
                ChangeState(L"JUMP_START");
            }
            else if (PLAYERCTRL->GetInput().Length() == 0.f)
            {
                ChangeState(L"RUN_END");
            }
            else if (!PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
            }
        }
        break;
        case ObjectCopyType::VENDING_MACHINE: {
            if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                UINT LeftCanCount = PLAYERFSM->GetCanCount();

                if (LeftCanCount > 0)
                {
                    if (KEY_NONE_ARROW)
                    {
                        ChangeState(L"ATTACK_START");
                    }
                    else
                    {
                        ChangeState(L"ATTACK_CHARGE1");
                    }
                }
                else
                {
                    ChangeState(L"ATTACK_FAILED");
                }
            }
            else if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
            {
                ChangeState(L"DROP_OBJECT_START");
            }
            else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
            {
                ChangeState(L"JUMP_START");
            }
            else if (PLAYERCTRL->GetInput().Length() == 0.f)
            {
                ChangeState(L"RUN_END");
            }
            else if (!PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
            }
        }
        break;
        case ObjectCopyType::LIGHT: {
            if (KEY_TAP(KEY_ATK) && !PLAYERFSM->IsAttackEvent())
            {
                ChangeState(L"ATTACK");
            }
            else if ((KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK)) && PLAYERFSM->IsAttackEvent())
            {
                ChangeState(L"ATTACK_END");
            }
            else if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
            {
                ChangeState(L"DROP_OBJECT");
            }
            else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
            {
                ChangeState(L"JUMP_START");
            }
            else if (PLAYERCTRL->GetInput().Length() == 0.f && KEY_NONE_ARROW)
            {
                ChangeState(L"RUN_END");
            }
            else if (!PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
            }
        }
        break;
        }
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::NORMAL: {
            if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                ChangeState(L"VACUUM1_START");
            }
            else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
            {
                ChangeState(L"JUMP_START");
            }
            else if (KEY_TAP(KEY_GUARD) || KEY_PRESSED(KEY_GUARD))
            {
                ChangeState(L"GUARD");
            }
            else if (PLAYERCTRL->GetInput().Length() == 0.f)
            {
                ChangeState(L"IDLE");
            }
            else if (!PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
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
                ChangeState(L"ATTACK_CHARGE1_START");
            }
            else if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
            {
                ChangeState(L"DROP_ABILITY");
            }
            else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
            {
                ChangeState(L"JUMP_START");
            }
            else if (KEY_TAP(KEY_GUARD) || KEY_PRESSED(KEY_GUARD))
            {
                ChangeState(L"GUARD");
            }
            else if (PLAYERCTRL->GetInput().Length() == 0.f)
            {
                ChangeState(L"IDLE");
            }
            else if (!PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
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
            else if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
            {
                ChangeState(L"DROP_ABILITY");
            }
            else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
            {
                ChangeState(L"JUMP_START");
            }
            else if (KEY_TAP(KEY_GUARD) || KEY_PRESSED(KEY_GUARD))
            {
                ChangeState(L"GUARD");
            }
            else if (PLAYERCTRL->GetInput().Length() == 0.f)
            {
                ChangeState(L"IDLE");
            }
            else if (!PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
            }
        }

        break;
        case AbilityCopyType::SWORD: {
            if (KEY_TAP(KEY_ATK) && PLAYERFSM->IsNearDeformObject())
            {
                ChangeState(L"VACUUM1_START");
            }
            else if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                UINT Combo = PLAYERFSM->GetComboLevel();

                if (Combo == 0)
                    ChangeState(L"ATTACK");
                else if (Combo == 1)
                    ChangeState(L"ATTACK_COMBO1");
                else if (Combo == 2)
                    ChangeState(L"ATTACK_COMBO2");
            }
            else if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
            {
                ChangeState(L"DROP_ABILITY");
            }
            else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
            {
                ChangeState(L"JUMP_START");
            }
            else if (KEY_TAP(KEY_GUARD) || KEY_PRESSED(KEY_GUARD))
            {
                ChangeState(L"GUARD");
            }
            else if (PLAYERCTRL->GetInput().Length() == 0.f)
            {
                ChangeState(L"IDLE");
            }
            else if (!PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
            }
        }
        break;
        case AbilityCopyType::SLEEP: {
            if (KEY_TAP(KEY_ATK) && PLAYERFSM->IsNearDeformObject())
            {
                ChangeState(L"VACUUM1_START");
            }
            else if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                ChangeState(L"ATTACK_START");
            }
            else if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
            {
                ChangeState(L"DROP_ABILITY");
            }
            else if (!PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
            }
            else if (PLAYERCTRL->GetInput().Length() == 0.f)
            {
                ChangeState(L"IDLE_START");
            }
        }
        break;
        }
    }
}

void CKirbyRun::Enter()
{
    m_FirstStep = true;

    SettingSmoke();

    PLAY_CURSTATE(RunEnter)
    PLAYERFSM->SetDroppable(true);
}

void CKirbyRun::Exit()
{
    PLAY_CURSTATE(RunExit)
    PLAYERFSM->SetDroppable(false);
}

void CKirbyRun::SettingSmoke()
{
    ObjectCopyType CurType = PLAYERFSM->GetCurObjectIdx();

    // 오브젝트 변신을 안했을 경우 Enter시에 세팅해놓은 값대로 스폰한다.
    if (CurType == ObjectCopyType::NONE)
    {
        m_FirstStepSmokeFrm = 6;
        m_SecondStepSmokeFrm = 26;

        m_SmokeOffset = 4.f;

        return;
    }

    switch (CurType)
    {
    case ObjectCopyType::CONE: {
        m_FirstStepSmokeFrm = 0;
        m_SecondStepSmokeFrm = 9;

        m_SmokeOffset= -26.f;
    }
    break;
    case ObjectCopyType::VENDING_MACHINE: {
        m_FirstStepSmokeFrm = 3;
        m_SecondStepSmokeFrm = 19;

        m_SmokeOffset = -30.f;


    }
    break;
    case ObjectCopyType::LIGHT: {
        m_FirstStepSmokeFrm = 6;
        m_SecondStepSmokeFrm = 18;

        m_SmokeOffset = -4.f;
    }
    break;
    default:
        break;
    }

}

void CKirbyRun::SpawnSmoke()
{
    AbilityCopyType CurType = PLAYERFSM->GetCurAbilityIdx();

    if (CurType == AbilityCopyType::SLEEP)
        return;

    if (m_FirstStep == true && CHECK_ANIMFRM(GetOwner(), m_FirstStepSmokeFrm) && CHECK_ANIMFRM_UNDER(GetOwner(), m_SecondStepSmokeFrm - 1))
    {
        GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Run.wav", 1, KIRBY_EFFECTSOUND);

        Ptr<CPrefab> Smoke = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Smoke.pref");
        if (Smoke.Get())
        {
            CGameObject* SmokeObj = Smoke->Instantiate();

            Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();
            Vec3 PlayerDir = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            Vec3 PlayerDirRight = PLAYER->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

            Vec3 SmokePos = PlayerPos - PlayerDir * 5.f;
            SmokePos.y += 5.f;
            SmokePos += PlayerDirRight * m_SmokeOffset;

            SmokeObj->Transform()->SetWorldPos(SmokePos);
            SmokeObj->Transform()->SetDirection(-PlayerDir);

            GamePlayStatic::SpawnGameObject(SmokeObj, LAYER_EFFECT);
        }

        m_FirstStep = false;
    }

    if (m_FirstStep == false && CHECK_ANIMFRM(GetOwner(), m_SecondStepSmokeFrm))
    {
        GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Run.wav", 1, KIRBY_EFFECTSOUND);

        Ptr<CPrefab> Smoke = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Smoke.pref");
        if (Smoke.Get())
        {
            CGameObject* SmokeObj = Smoke->Instantiate();

            Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();
            Vec3 PlayerDir = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            Vec3 PlayerDirRight = PLAYER->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

            Vec3 SmokePos = PlayerPos - PlayerDir * 5.f;
            SmokePos.y += 5.f;
            SmokePos -= PlayerDirRight * m_SmokeOffset;

            SmokeObj->Transform()->SetWorldPos(SmokePos);
            SmokeObj->Transform()->SetDirection(-PlayerDir);

            GamePlayStatic::SpawnGameObject(SmokeObj, LAYER_EFFECT);
        }

        m_FirstStep = true;
    }
}
