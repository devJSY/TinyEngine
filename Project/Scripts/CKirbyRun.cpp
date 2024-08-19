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
    // 6프레임 -> 오른발, 26프레임 ->왼발
    if (m_LastSmokeIsRight == false && CHECK_ANIMFRM(GetOwner(), 6) && CHECK_ANIMFRM_UNDER(GetOwner(), 25))
    {
        Ptr<CPrefab> Smoke = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Smoke.pref");
        if (Smoke.Get())
        {
            CGameObject* SmokeObj = Smoke->Instantiate();

            Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();
            Vec3 PlayerDir = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            Vec3 PlayerDirRight = PLAYER->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

            Vec3 SmokePos = PlayerPos - PlayerDir * 5.f;
            SmokePos.y += 5.f;
            SmokePos += PlayerDirRight * 4.f;

            SmokeObj->Transform()->SetWorldPos(SmokePos);
            SmokeObj->Transform()->SetDirection(-PlayerDir);

            GamePlayStatic::SpawnGameObject(SmokeObj, LAYER_EFFECT);
        }

        m_LastSmokeIsRight = true;
    }

    if (m_LastSmokeIsRight == true && CHECK_ANIMFRM(GetOwner(), 26))
    {
        Ptr<CPrefab> Smoke = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Smoke.pref");
        if (Smoke.Get())
        {
            CGameObject* SmokeObj = Smoke->Instantiate();

            Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();
            Vec3 PlayerDir = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            Vec3 PlayerDirRight = PLAYER->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

            Vec3 SmokePos = PlayerPos - PlayerDir * 5.f;
            SmokePos.y += 5.f;
            SmokePos -= PlayerDirRight * 4.f;

            SmokeObj->Transform()->SetWorldPos(SmokePos);
            SmokeObj->Transform()->SetDirection(-PlayerDir);

            GamePlayStatic::SpawnGameObject(SmokeObj, LAYER_EFFECT);
        }

        m_LastSmokeIsRight = false;
    }



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
                    ChangeState(L"ATTACK_START");
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
    m_LastSmokeIsRight = false;

    PLAY_CURSTATE(RunEnter)
    PLAYERFSM->SetDroppable(true);
}

void CKirbyRun::Exit()
{
    PLAY_CURSTATE(RunExit)
    PLAYERFSM->SetDroppable(false);
}