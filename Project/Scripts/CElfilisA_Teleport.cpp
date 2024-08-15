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
    , m_EffectSpeed(500.f)
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

void CElfilisA_Teleport::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Wait"));
        GetOwner()->Animator()->SetPlay(false);
        GetOwner()->Rigidbody()->SetKinematic(true);

        //@Effect 일부분만 그리는 셰이더 작성 필요

        // copy object
        m_BeforeObj = new CGameObject;
        m_BeforeObj->AddComponent(GetOwner()->Transform()->Clone());
        m_BeforeObj->AddComponent(GetOwner()->MeshRender()->Clone());
        m_BeforeObj->AddComponent(GetOwner()->Animator()->Clone());
        m_BeforeObj->SetName(L"Effect_ElfilisTelport Body");
        GamePlayStatic::SpawnGameObject(m_BeforeObj, LAYER_MONSTER);

        // teleport
        float MapSizeRadius = ELFFSM->GetMapSizeRadius();
        Vec3 MapFloorOffset = ELFFSM->GetMapFloorOffset();
        m_AfterPos = PLAYER->Transform()->GetWorldPos() + PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100.f;

        if (m_AfterPos.x < 0)
        {
            if (m_AfterPos.x < MapSizeRadius * -1.f + MapFloorOffset.x)
            {
                m_AfterPos.x = MapSizeRadius * -1.f;
            }
        }
        else
        {
            if (m_AfterPos.x > MapSizeRadius + MapFloorOffset.x)
            {
                m_AfterPos.x = MapSizeRadius;
            }
        }

        if (m_AfterPos.z < 0)
        {
            if (m_AfterPos.z < MapSizeRadius * -1.f + MapFloorOffset.z)
            {
                m_AfterPos.z = MapSizeRadius * -1.f;
            }
        }
        else
        {
            if (m_AfterPos.z > MapSizeRadius + MapFloorOffset.z)
            {
                m_AfterPos.z = MapSizeRadius;
            }
        }

        //@Effect 텔레포드 이펙트
        Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
        Pos.y += 100.f;
        m_BeforeEffect = m_Effect->Instantiate();
        m_BeforeEffect->Transform()->SetWorldPos(Pos);
        GamePlayStatic::SpawnGameObject(m_BeforeEffect, LAYER_EFFECT);

        Pos = m_AfterPos;
        Pos.y += 100.f;
        m_AfterEffect = m_Effect->Instantiate();
        m_AfterEffect->Transform()->SetWorldPos(Pos);
        GamePlayStatic::SpawnGameObject(m_AfterEffect, LAYER_EFFECT);
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
    case StateStep::Start: {
        GetOwner()->Rigidbody()->SetKinematic(false);
    }
    break;
    case StateStep::End: {
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

    Pos = m_AfterEffect->Transform()->GetWorldPos();
    Pos.y -= m_EffectSpeed * DT;
    m_AfterEffect->Transform()->SetWorldPos(Pos);

    if (Pos.y <= 0.f)
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();
        ELFFSM->ChangeStateGroup(NextState);
    }
}