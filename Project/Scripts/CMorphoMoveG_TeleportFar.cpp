#include "pch.h"
#include "CMorphoMoveG_TeleportFar.h"
#include "CMorphoFSM.h"

CMorphoMoveG_TeleportFar::CMorphoMoveG_TeleportFar()
    : m_AccTime(0.f)
    , m_WaitTime(0.5f)
    , m_bParticleSpawn(false)
{
}

CMorphoMoveG_TeleportFar::~CMorphoMoveG_TeleportFar()
{
}

void CMorphoMoveG_TeleportFar::tick()
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

void CMorphoMoveG_TeleportFar::Exit()
{
    Exit_Step();

    MRPFSM->EnableRender();
    MRPFSM->EnableCollider();
    GetOwner()->Rigidbody()->SetUseGravity(true);
    GetOwner()->Animator()->SetPlay(true);
}

void CMorphoMoveG_TeleportFar::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->SetPlay(false);
        m_AccTime = 0.f;
        m_bParticleSpawn = false;

        // teleport pos
        Vec3 MapSize = MRPFSM->GetMapSize();
        m_AfterPos.x = GetRandomfloat(-MapSize.x, MapSize.x);
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

void CMorphoMoveG_TeleportFar::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        MRPFSM->DisableRender();
        MRPFSM->DisableCollider();
        MRPFSM->ResetEmissive();
        GetOwner()->Rigidbody()->SetUseGravity(false);
    }
    break;
    case StateStep::End:
        break;
    }
}

void CMorphoMoveG_TeleportFar::Start()
{
    m_AccTime += DT;

    // Disappear
    float delta = m_AccTime / MRPFSM->GetEmissiveTime();
    float t1 = delta;
    float t2 = sinf(delta * XM_PI / 2.f);
    Vec3 Color = Vec3(t1);
    Color.x = t2;

    MRPFSM->AddEmissive(Color);

    // Particle On
    if (!m_bParticleSpawn && m_AccTime > MRPFSM->GetEmissiveTime() - 0.25f)
    {
        m_bParticleSpawn = true;
        MRPFSM->EnableTeleportParticle(true);
    }

    if (m_AccTime > MRPFSM->GetEmissiveTime())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoMoveG_TeleportFar::End()
{
    m_AccTime += DT;

    // Particle Off
    if (m_bParticleSpawn && m_AccTime > 0.25f)
    {
        m_bParticleSpawn = false;
        MRPFSM->EnableTeleportParticle(false);
    }

    if (m_AccTime > m_WaitTime)
    {
        MRPFSM->SetTeleportTime(true);
        MRPFSM->Attack();
    }
}