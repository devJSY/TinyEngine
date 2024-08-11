#include "pch.h"
#include "CMorphoMoveA_Teleport.h"
#include "CMorphoFSM.h"

CMorphoMoveA_Teleport::CMorphoMoveA_Teleport()
    : m_AccTime(0.f)
    , m_WaitTime(0.5f)
{
}

CMorphoMoveA_Teleport::~CMorphoMoveA_Teleport()
{
}

void CMorphoMoveA_Teleport::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CMorphoMoveA_Teleport::Exit()
{
    Exit_Step();

    GetOwner()->Rigidbody()->SetUseGravity(true);
}

void CMorphoMoveA_Teleport::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->SetPlay(false);
        GetOwner()->Rigidbody()->SetUseGravity(false);
        m_AccTime = 0.f;

        // teleport pos
        Vec3 MapSize = MRPFSM->GetMapSize();
        m_AfterPos.x = GetRandomfloat(-MapSize.x, MapSize.x);
        m_AfterPos.y = 80.f;
        m_AfterPos.z = GetRandomfloat(-MapSize.z, MapSize.z);
        m_AfterPos += MRPFSM->GetMapFloorOffset();
    }
    break;
    case StateStep::End: {
        Vec3 Dir = PLAYER->Transform()->GetWorldPos() - m_AfterPos;
        Dir.y = 0.f;
        Dir.Normalize();

        GetOwner()->Transform()->SetWorldPos(m_AfterPos);
        GetOwner()->Transform()->Slerp(Dir, 1.f);
        m_AccTime = 0.f;
    }
    break;
    }
}

void CMorphoMoveA_Teleport::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        MRPFSM->DisableRender();
        MRPFSM->ClearEmissive();
    }
    break;
    case StateStep::End: {
        MRPFSM->EnableRender();
        GetOwner()->Animator()->SetPlay(true);
        GetOwner()->Rigidbody()->SetUseGravity(true);
    }
    break;
    }
}

void CMorphoMoveA_Teleport::Start()
{
    m_AccTime += DT;

    // Disappear
    float delta = m_AccTime / MRPFSM->GetEmissiveTime();
    float t1 = delta;
    float t2 = sinf(delta * XM_PI / 2.f);
    Vec3 Color = Vec3(t1);
    Color.x = t2;

    MRPFSM->SetEmissive(Color);

    if (m_AccTime > MRPFSM->GetEmissiveTime())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoMoveA_Teleport::End()
{
    m_AccTime += DT;

    if (m_AccTime > m_WaitTime)
    {
        MRPFSM->SetTeleportTime(true);
        MRPFSM->Attack();
    }
}