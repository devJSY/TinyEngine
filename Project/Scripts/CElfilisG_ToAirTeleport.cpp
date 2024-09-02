#include "pch.h"
#include "CElfilisG_ToAirTeleport.h"
#include "CElfilisFSM.h"
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

#include "CCameraController.h"

CElfilisG_ToAirTeleport::CElfilisG_ToAirTeleport()
    : m_BeforeObj(nullptr)
    , m_BeforeEffect(nullptr)
    , m_AfterEffect(nullptr)
    , m_EffectSpeed(200.f)
{
    m_Effect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_ElfilisTeleport.pref", L"prefab\\Effect_ElfilisTeleport.pref");
}

CElfilisG_ToAirTeleport::~CElfilisG_ToAirTeleport()
{
}

void CElfilisG_ToAirTeleport::tick()
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

void CElfilisG_ToAirTeleport::Exit()
{
    Exit_Step();

    ELFFSM->ReleaseDynamicMtrl();
    GetOwner()->Animator()->SetPlay(true);

    if (m_BeforeObj)
    {
        GamePlayStatic::DestroyGameObject(m_BeforeObj);
    }
    if (m_BeforeEffect)
    {
        GamePlayStatic::DestroyGameObject(m_BeforeEffect);
    }
    if (m_AfterEffect)
    {
        GamePlayStatic::DestroyGameObject(m_AfterEffect);
    }
}

void CElfilisG_ToAirTeleport::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Wait"));
        GetOwner()->Animator()->SetPlay(false);

        SpawnTeleport();

        // Camera : 하늘
        CAMERACTRL->SetElfilisSky();
    }
    break;
    case StateStep::End:
        break;
    }
}

void CElfilisG_ToAirTeleport::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::End:
        break;
    }
}

void CElfilisG_ToAirTeleport::Start()
{
    // Teleport (After 1 tick : Spawn 생성 기다림)
    Vec3 Dir = PLAYER->Transform()->GetWorldPos() - m_AfterPos;
    Dir.y = 0.f;
    Dir.Normalize();

    GetOwner()->Transform()->Slerp(Dir, 1.f);
    GetOwner()->Transform()->SetWorldPos(m_AfterPos);

    ChangeStep(StateStep::End);
}

void CElfilisG_ToAirTeleport::End()
{
    // move effect
    Vec3 Pos = m_AfterEffect->Transform()->GetWorldPos();
    Pos.y -= m_EffectSpeed * DT;
    m_AfterEffect->Transform()->SetWorldPos(Pos);
    ELFFSM->Teleport(m_BeforeObj, 2, Pos.y);

    Pos = m_BeforeEffect->Transform()->GetWorldPos();
    Pos.y -= m_EffectSpeed * DT;
    m_BeforeEffect->Transform()->SetWorldPos(Pos);
    ELFFSM->Teleport(1, Pos.y);

    if (Pos.y <= 0.f)
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();
        ELFFSM->ChangeStateGroup(NextState);
    }
}

void CElfilisG_ToAirTeleport::SpawnTeleport()
{
    // teleport
    if (!ELFFSM->IsPattern(ElfilisPatternType::BigCombo, 1))
    {
        Vec3 RandDir = Vec3(GetRandomfloat(-100.f, 100.f), 0.f, GetRandomfloat(-100.f, 100.f)).Normalize();
        if (RandDir.Length() <= 0.f)
        {
            RandDir = Vec3(1.f, 0.f, 1.f);
        }
        Vec3 RandAirPos = RandDir * ELFFSM->GetAirPos().z + Vec3(0.f, ELFFSM->GetAirPos().y, 0.f);
        m_AfterPos = RandAirPos;
    }
    else
    {
        Vec3 TargetDir = ELFFSM->GetBigBoss()->Transform()->GetWorldPos() - ELFFSM->GetMapFloorOffset();
        TargetDir.y = 0.f;
        TargetDir.Normalize();

        Vec3 NewPos = TargetDir * ELFFSM->GetAirPos().z + Vec3(0.f, ELFFSM->GetAirPos().y, 0.f);
        m_AfterPos = NewPos;
    }

    ELFFSM->GetUnit()->PlayTeleportEffect(&m_BeforeObj, &m_BeforeEffect, &m_AfterEffect, m_AfterPos);
}
