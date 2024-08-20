#include "pch.h"
#include "CElfilisD_Resist.h"
#include "CElfilisFSM.h"
#include "CCameraController.h"
#include "CFlowMgr_BossElfilis.h"

CElfilisD_Resist::CElfilisD_Resist()
    : m_AccTime(0.f)
{
}

CElfilisD_Resist::~CElfilisD_Resist()
{
}

void CElfilisD_Resist::tick()
{
    switch (m_Step)
    {
    case StateStep::ReadyStart: {
        ReadyStart();
    }
    break;
    case StateStep::Ready: {
        Ready();
    }
    break;
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Progress: {
        Progress();
    }
    break;
    }
}

void CElfilisD_Resist::Enter()
{
    m_Step = StateStep::ReadyStart;
    Enter_Step();
}

void CElfilisD_Resist::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::ReadyStart: {
        GetOwner()->Animator()->Play(ANIMPREFIX("LastDamageStart"), false, false, 1.5f, 0.f);
        GetOwner()->Transform()->SetWorldPos(ELFFSM->GetMapFloorOffset());
        CBossMgr::GetElfilisFlowMgr()->ChangeFlowResist();
        //@CAMERA 에피리스 타겟 (진입위치, 각도 등 항상 같음)
    }
    break;
    case StateStep::Ready: {
        GetOwner()->Animator()->Play(ANIMPREFIX("LastDamageWait"), false, false, 1.5f);
        m_AccTime = 0.f;

        //@CAMERA 줌인
    }
    break;
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ResistStart"), false, false, 1.5f);

        //@CAMERA 투타겟
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ResistLoop"), true, true, 1.5f, 0.3f);
        m_AccTime += DT;
    }
    break;
    }
}

void CElfilisD_Resist::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::ReadyStart:
        break;
    case StateStep::Ready:
        break;
    case StateStep::Start:
        break;
    case StateStep::Progress: {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());
    }
    break;
    }
}

void CElfilisD_Resist::ReadyStart()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Ready);
    }
}

void CElfilisD_Resist::Ready()
{
    m_AccTime += DT;

    if (m_AccTime > 1.f)
    {
        ChangeStep(StateStep::Start);
    }
}

void CElfilisD_Resist::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisD_Resist::Progress()
{
    m_AccTime += DT;
    RotateToPlayer(DT * 0.3f);

    // Move
    float Speed = 0.5f;
    Vec3 Force = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -1.f;
    Force.y = 0.f;
    Force.Normalize();

    if (GetOwner()->Rigidbody()->GetVelocity().Length() >= Speed)
    {
        Force *= Speed;
        GetOwner()->Rigidbody()->SetVelocity(Force);
    }
    else
    {
        Force *= (Speed * DT + GetOwner()->Rigidbody()->GetVelocity().Length());
        GetOwner()->Rigidbody()->SetVelocity(Force);
    }

    // Change State
    bool bSuccess = (BOSSUNIT->GetCurInfo().HP <= 0.f);

    if (bSuccess)
    {
        ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_RESIST_FAIL");
    }
    else
    {
        if (m_AccTime > 10.f)
        {
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_RESIST_SUCCESS");
        }
    }
}