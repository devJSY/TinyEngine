#include "pch.h"
#include "CElfilisG_NormalAtkR.h"
#include "CElfilisFSM.h"

CElfilisG_NormalAtkR::CElfilisG_NormalAtkR()
    : m_bComboSuccess(false)
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

void CElfilisG_NormalAtkR::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRightStart"), false, false, 2.5f, 0.5f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRight"), false);
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
            ELFFSM->SetComboLevel(0);
        }
    }
    break;
    case StateStep::End:
        break;
    }
}

void CElfilisG_NormalAtkR::Start()
{
    // rotate
    Vec3 Dir = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
    Dir.y = 0.f;
    Dir.Normalize();
    GetOwner()->Transform()->Slerp(Dir, 1.f);

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_NormalAtkR::Progress()
{
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

            if (ELFFSM->GetPhase() == 1 || (ELFFSM->GetPhase() == 2) && ELFFSM->GetComboLevel() == 2)
            {
                ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundAtk, L"GROUND_ATK_NORMAL_FINISHL");
            }
            else
            {
                ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundAtk, L"GROUND_ATK_NORMAL_L");
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
            if (ELFFSM->GetPhase() == 1)
            {
                ELFFSM->RepeatState(L"GROUND_ATK_NORMAL_L");
            }
            else
            {
                ELFFSM->RepeatState(L"GROUND_ATK_NORMAL_R");
            }
        }
        else
        {
            ELFFSM->ChangeStateGroup_RandState(NextState);
        }
    }
}