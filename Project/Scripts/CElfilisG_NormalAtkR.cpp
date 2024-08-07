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
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRightStart"), false, false, 1.f, 0.5f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRight"), false, false, 1.f);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRightEnd"), false, false, 1.f);
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