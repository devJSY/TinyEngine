#include "pch.h"
#include "CKirbyRunStart.h"

CKirbyRunStart::CKirbyRunStart()
{
}

CKirbyRunStart::~CKirbyRunStart()
{
}

void CKirbyRunStart::tick()
{
    SpawnSmoke();

    PLAY_CURSTATE(RunStart)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::NORMAL: {
            // 상태변경 시그널 발생시
            if (KEY_TAP(KEY_ATK))
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
            else if (PLAYERCTRL->GetInput().Length() == 0.f && KEY_NONE_ARROW)
            {
                ChangeState(L"IDLE");
            }
            else if (!PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
            }
            // 상태변경 시그널 발생 않을 시
            else
            {
                if (GetOwner()->Animator()->IsFinish())
                    ChangeState(L"RUN");
            }
        }
        break;
        case AbilityCopyType::FIRE: {
            if (KEY_TAP(KEY_ATK) && PLAYERFSM->IsNearDeformObject())
            {
                ChangeState(L"VACUUM1_START");
            }
            else if (KEY_TAP(KEY_ATK))
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
            else if (PLAYERCTRL->GetInput().Length() == 0.f && KEY_NONE_ARROW)
            {
                ChangeState(L"IDLE");
            }
            else if (!PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
            }
            else
            {
                if (GetOwner()->Animator()->IsFinish())
                    ChangeState(L"RUN");
            }
        }
        break;

        case AbilityCopyType::CUTTER: {
            // 상태변경 시그널 발생시
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
            // 상태변경 시그널 발생 않을 시
            else
            {
                if (GetOwner()->Animator()->IsFinish())
                    ChangeState(L"RUN");
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
            // 상태변경 시그널 발생 않을 시
            else
            {
                if (GetOwner()->Animator()->IsFinish())
                    ChangeState(L"RUN");
            }
        }
        break;
        }
    }
}

void CKirbyRunStart::Enter()
{
    m_LastSmokeIsRight = false;

    PLAY_CURSTATE(RunStartEnter)
    PLAYERFSM->SetDroppable(true);
}

void CKirbyRunStart::Exit()
{
    PLAY_CURSTATE(RunStartExit)
    PLAYERFSM->SetDroppable(false);
}

void CKirbyRunStart::SpawnSmoke()
{
    AbilityCopyType CurType = PLAYERFSM->GetCurAbilityIdx();

    if (CurType == AbilityCopyType::SLEEP)
        return;

    // 6프레임 -> 오른발, 26프레임 ->왼발
    if (m_LastSmokeIsRight == false && CHECK_ANIMFRM(GetOwner(), 6) && CHECK_ANIMFRM_UNDER(GetOwner(), 25))
    {
        GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Run.wav", 1, 50.f);

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
        GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Run.wav", 1, 50.f);

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
}
