#include "pch.h"
#include "CElfilisG_NormalAtkR.h"
#include "CElfilisFSM.h"
#include "CCameraController.h"

CElfilisG_NormalAtkR::CElfilisG_NormalAtkR()
    : m_bComboSuccess(false)
    , m_bFrmEnter(true)
{
}

CElfilisG_NormalAtkR::~CElfilisG_NormalAtkR()
{
}

void CElfilisG_NormalAtkR::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Progress: {
        Progress();
    }
    break;
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CElfilisG_NormalAtkR::Exit()
{
    Exit_Step();

    if (m_Step < StateStep::Progress)
    {
        ELFFSM->ClearComboLevel();
        ELFFSM->OffWeaponTrigger();
    }
}

void CElfilisG_NormalAtkR::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRightStart"), false, false, 1.5f, 0.5f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRight"), false, false, 1.5f);
        ELFFSM->OnWeaponTrigger();
        m_bFrmEnter = true;
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRightEnd"), false, false, 1.5f);
    }
    break;
    }
}

void CElfilisG_NormalAtkR::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress: {
        if (m_bComboSuccess)
        {
            ELFFSM->AddComboLevel();
        }
        else
        {
            ELFFSM->ClearComboLevel();
        }
        ELFFSM->OffWeaponTrigger();

        // Spawn DropStar
        Vec3 SpawnDir = -(GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT) + GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT)).Normalize();
        Vec3 SpawnPos = GetOwner()->Transform()->GetWorldPos() + SpawnDir * 100.f;
        ELFFSM->SpawnDropStar(SpawnPos);
    }
    break;
    case StateStep::End:
        break;
    }
}

void CElfilisG_NormalAtkR::Start()
{
    // rotate
    RotateToPlayer();

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_NormalAtkR::Progress()
{
    // resize Hitbox
    CBoxCollider* pHitbox = ELFFSM->GetHitbox();
    if (pHitbox && !GetOwner()->Animator()->IsChainging())
    {
        if (GetOwner()->Animator()->GetClipFrameIndex() < 13)
        {
            if (m_bFrmEnter)
            {
                m_bFrmEnter = false;

                pHitbox->GetOwner()->SetActive(true);
                pHitbox->Transform()->SetLocalPos(Vec3(0.71f, 1.f, 2.64f));
                pHitbox->Transform()->SetLocalRotation(Vec3(0.f));
                pHitbox->Transform()->SetLocalScale(Vec3(7.56f, 1.f, 4.81f));

                CAMERACTRL->Shake(0.3f, 30.f, 30.f);
            }
        }
        else
        {
            pHitbox->GetOwner()->SetActive(false);
        }
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        // Check Combo
        float Rand = 0.f;
        if (ELFFSM->GetPhase() == 1)
        {
            Rand = GetRandomfloat(1.f, 100.f);
        }

        if (Rand <= 90)
        {
            m_bComboSuccess = true;

            if (ELFFSM->GetPhase() == 1 || (ELFFSM->GetPhase() == 2 && ELFFSM->GetComboLevel() == 2))
            {
                if (ELFFSM->IsNearPlayer())
                {
                    ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundAtkNear, L"GROUND_ATK_NORMAL_FINISHL");
                }
                else
                {
                    ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundAtkNear, L"GROUND_ATK_NORMALTELEPORT_FINISHL");
                }
            }
            else
            {
                if (ELFFSM->IsNearPlayer())
                {
                    ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundAtkNear, L"GROUND_ATK_NORMAL_L");
                }
                else
                {
                    ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundAtkNear, L"GROUND_ATK_NORMALTELEPORT_L");
                }
            }
        }
        else
        {
            ChangeStep(StateStep::End);
        }
    }
}

void CElfilisG_NormalAtkR::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();

        if (NextState == ELFFSM->GetCurStateGroup())
        {
            ELFFSM->RepeatState(L"GROUND_ATK_NORMAL");
        }
        else
        {
            ELFFSM->ChangeStateGroup(NextState);
        }
    }
}