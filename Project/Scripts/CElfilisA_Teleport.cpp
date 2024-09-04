#include "pch.h"
#include "CElfilisA_Teleport.h"
#include "CElfilisFSM.h"
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

#include "CCameraController.h"

CElfilisA_Teleport::CElfilisA_Teleport()
    : m_BeforeObj(nullptr)
    , m_BeforeEffect(nullptr)
    , m_AfterEffect(nullptr)
    , m_EffectSpeed(200.f)
{
    m_Effect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_ElfilisTeleport.pref", L"prefab\\Effect_ElfilisTeleport.pref");
}

CElfilisA_Teleport::~CElfilisA_Teleport()
{
}

void CElfilisA_Teleport::tick()
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

void CElfilisA_Teleport::Exit()
{
    Exit_Step();

    ELFFSM->ReleaseDynamicMtrl();
    ELFFSM->EnableCollider();
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

    // 투 타겟
    CAMERACTRL->SetElfilisTwoTarget();
}

void CElfilisA_Teleport::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Wait"));
        GetOwner()->Animator()->SetPlay(false);

        // teleport
        float MapSizeRadius = ELFFSM->GetMapSizeRadius();
        Vec3 MapFloorOffset = ELFFSM->GetMapFloorOffset();
        m_AfterPos = PLAYER->Transform()->GetWorldPos() + PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100.f;
        Vec3 NewPosMapDist = m_AfterPos - MapFloorOffset;

        if (NewPosMapDist.Length() > MapSizeRadius)
        {
            Vec3 Dir = NewPosMapDist.Normalize();
            m_AfterPos = MapFloorOffset + Dir * MapSizeRadius;
        }

        ELFFSM->GetUnit()->PlayTeleportEffect(&m_BeforeObj, &m_BeforeEffect, &m_AfterEffect, m_AfterPos);
    }
    break;
    case StateStep::End:
        break;
    }
}

void CElfilisA_Teleport::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::End:
        break;
    }
}

void CElfilisA_Teleport::Start()
{
    // Teleport (After 1 tick : Spawn 생성 기다림)
    Vec3 Dir = PLAYER->Transform()->GetWorldPos() - m_AfterPos;
    Dir.y = 0.f;
    Dir.Normalize();

    GetOwner()->Transform()->Slerp(Dir, 1.f);
    GetOwner()->Transform()->SetWorldPos(m_AfterPos);

    ChangeStep(StateStep::End);
}

void CElfilisA_Teleport::End()
{
    // move effect
    Vec3 Pos = m_BeforeEffect->Transform()->GetWorldPos();
    Pos.y -= m_EffectSpeed * DT;
    m_BeforeEffect->Transform()->SetWorldPos(Pos);
    ELFFSM->Teleport(m_BeforeObj, 2, Pos.y);

    Pos = m_AfterEffect->Transform()->GetWorldPos();
    Pos.y -= m_EffectSpeed * DT;
    m_AfterEffect->Transform()->SetWorldPos(Pos);
    ELFFSM->Teleport(1, Pos.y);

    if (Pos.y <= 0.f)
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();
        ELFFSM->ChangeStateGroup(NextState);
    }
}