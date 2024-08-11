#include "pch.h"
#include "CMorphoMoveG_TeleportNear.h"
#include "CMorphoFSM.h"

CMorphoMoveG_TeleportNear::CMorphoMoveG_TeleportNear()
    : m_AccTime(0.f)
    , m_WaitTime(0.5f)
{
}

CMorphoMoveG_TeleportNear::~CMorphoMoveG_TeleportNear()
{
}

void CMorphoMoveG_TeleportNear::tick()
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

void CMorphoMoveG_TeleportNear::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->SetPlay(false);
        m_AccTime = 0.f;

        // teleport pos
        Vec3 MapSize = MRPFSM->GetMapSize();
        Vec3 MapOffset = MRPFSM->GetMapFloorOffset();
        Vec3 Dist = Vec3(GetRandomfloat(-100.f, 100.f), 0.f, GetRandomfloat(-100.f, 100.f)).Normalize();
        if (Dist.Length() <= 0.f)
        {
            Dist = Vec3(1.f, 0.f, 1.f).Normalize();
        }

        Dist *= MRPFSM->GetNearDist() * (GetRandomfloat(20.f, 100.f) / 100.f);
        m_AfterPos = PLAYER->Transform()->GetWorldPos() + Dist;
        m_AfterPos.y = 0.f;

        if (m_AfterPos.x < 0 && m_AfterPos.x < -MapSize.x + MapOffset.x)
        {
            m_AfterPos.x = -MapSize.x + MapOffset.x;
        }
        else if (m_AfterPos.x > 0 && m_AfterPos.x > MapSize.x + MapOffset.x)
        {
            m_AfterPos.x = MapSize.x + MapOffset.x;
        }

        if (m_AfterPos.z < 0 && m_AfterPos.z < -MapSize.z + MapOffset.z)
        {
            m_AfterPos.z = -MapSize.z + MapOffset.z;
        }
        else if (m_AfterPos.z > 0 && m_AfterPos.z > MapSize.z)
        {
            m_AfterPos.z = MapSize.z + MapOffset.z;
        }
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

void CMorphoMoveG_TeleportNear::Exit_Step()
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
    }
    break;
    }
}

void CMorphoMoveG_TeleportNear::Start()
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

void CMorphoMoveG_TeleportNear::End()
{
    m_AccTime += DT;

    if (m_AccTime > m_WaitTime)
    {
        MRPFSM->SetTeleportTime(true);
        MRPFSM->Attack();
    }
}