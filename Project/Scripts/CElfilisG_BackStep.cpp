#include "pch.h"
#include "CElfilisG_BackStep.h"
#include "CElfilisFSM.h"

CElfilisG_BackStep::CElfilisG_BackStep()
    : m_PrevDrag(0.f)
    , m_bFinishMove(false)
    , m_bWall(false)
{
}

CElfilisG_BackStep::~CElfilisG_BackStep()
{
}

void CElfilisG_BackStep::tick()
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

void CElfilisG_BackStep::Exit()
{
    Exit_Step();

    GetOwner()->Rigidbody()->SetVelocity(Vec3());
    GetOwner()->Rigidbody()->SetDrag(m_PrevDrag);
}

void CElfilisG_BackStep::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AwayFastReady"), false);
        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();
        m_bFinishMove = false;
        m_bWall = false;
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AwayFastStart"), false);

        m_ForceDir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -1.f;
        m_ForceDir.y = 0.f;
        m_ForceDir.Normalize();

        m_StartPos = GetOwner()->Transform()->GetWorldPos();
        m_TargetPos = m_StartPos + m_ForceDir * 400.f;
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AwayFastEnd"), false);
    }
    break;
    }
}

void CElfilisG_BackStep::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress:
        break;
    case StateStep::End:
        break;
    }
}

void CElfilisG_BackStep::Start()
{
    // look player
    RotateToPlayer(DT * 10.f);

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_BackStep::Progress()
{
    MoveBack();

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisG_BackStep::End()
{
    MoveBack();

    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();

        if (NextState == ElfilisStateGroup::GroundAtkFar || NextState == ElfilisStateGroup::GroundAtkNear)
        {
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundAtkFar, L"GROUND_ATK_SWORDWAVE_RL");
            // float Rand = GetRandomfloat(1.f, 100.f);

            // if (Rand <= 45.f)
            //{
            //     ELFFSM->ChangeStateGroup_SetState(NextState, L"GROUND_ATK_SWORDWAVE_RL");
            // }
            // else
            //{
            //     ELFFSM->ChangeStateGroup_SetState(NextState, L"GROUND_ATK_DIMENSIONSPIKE");
            // }
        }
        else
        {
            ELFFSM->ChangeStateGroup(NextState);
        }
    }
}

void CElfilisG_BackStep::MoveBack()
{
    if (m_bWall)
        return;

    // move
    if (!m_bFinishMove)
    {
        Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
        float CurDist = (NewPos - m_StartPos).Length();
        float Ratio = CurDist / (m_TargetPos - m_StartPos).Length();

        if (Ratio < 1.f)
        {
            Ratio = Ratio * 0.9f + 0.1f;
            float SinGraph = clamp(sinf(Ratio * XM_PI) * 1.5f, 0.f, 1.f);
            float NewSpeed = 40.f * SinGraph;

            GetOwner()->Rigidbody()->SetVelocity(m_ForceDir * NewSpeed);
        }
        else
        {
            if (!m_bFinishMove)
            {
                m_bFinishMove = true;
                GetOwner()->Rigidbody()->SetDrag(4.f);
            }
        }
    }

    // Map Check
    Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
    float MapSizeRadius = ELFFSM->GetMapSizeRadius();

    if (NewPos.Length() > MapSizeRadius)
    {
        Vec3 MapFloorOffset = ELFFSM->GetMapFloorOffset();
        Vec3 Dir = (NewPos - MapFloorOffset).Normalize();
        NewPos = MapFloorOffset + Dir * MapSizeRadius;

        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Transform()->SetWorldPos(NewPos);
        m_bWall = true;
    }
}