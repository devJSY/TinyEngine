#include "pch.h"
#include "CElfilisG_NormalAtkTeleportFinishL.h"
#include "CElfilisFSM.h"

CElfilisG_NormalAtkTeleportFinishL::CElfilisG_NormalAtkTeleportFinishL()
    : m_BeforeObj(nullptr)
    , m_BeforeEffect(nullptr)
    , m_AfterEffect(nullptr)
    , m_EffectSpeed(400.f)
    , m_bFrmEnter(true)
{
    m_Effect = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Effect_ElfilisTeleport.pref");
}

CElfilisG_NormalAtkTeleportFinishL::~CElfilisG_NormalAtkTeleportFinishL()
{
}

void CElfilisG_NormalAtkTeleportFinishL::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::StartEnd: {
        StartEnd();
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

void CElfilisG_NormalAtkTeleportFinishL::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingLeftStart"), false, false, 1.f, 0.5f);
    }
    break;
    case StateStep::StartEnd: {
        GetOwner()->Rigidbody()->SetKinematic(true);
        m_bFrmEnter = true;
        SpawnTeleport();
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingFinishLeft"), false, false, 1.f);
        ELFFSM->OnWeaponTrigger();
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingFinishLeftEnd"), false, false, 1.f);
    }
    break;
    }
}

void CElfilisG_NormalAtkTeleportFinishL::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::StartEnd: {
        GetOwner()->Rigidbody()->SetKinematic(false);

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
    break;
    case StateStep::Progress: {
        ELFFSM->OffWeaponTrigger();
    }
        break;
    case StateStep::End: {
        ELFFSM->ClearComboLevel();
    }
    break;
    }
}

void CElfilisG_NormalAtkTeleportFinishL::Start()
{
    // rotate
    RotateToPlayer();

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::StartEnd);
    }
}

void CElfilisG_NormalAtkTeleportFinishL::StartEnd()
{
    if (m_bFrmEnter)
    {
        // Teleport (After 1 tick : Spawn 생성 기다림)
        Vec3 Dir = PLAYER->Transform()->GetWorldPos() - m_AfterPos;
        Dir.y = 0.f;
        Dir.Normalize();

        GetOwner()->Transform()->Slerp(Dir, 1.f);
        GetOwner()->Transform()->SetWorldPos(m_AfterPos);

        m_bFrmEnter = false;
    }
    else
    {
        // move effect
        Vec3 Pos = m_BeforeEffect->Transform()->GetWorldPos();
        float ChangeHeight = Pos.y - m_EffectSpeed * DT;
        Pos.y = ChangeHeight;
        m_BeforeEffect->Transform()->SetWorldPos(Pos);

        Pos = m_AfterEffect->Transform()->GetWorldPos();
        Pos.y = ChangeHeight;
        m_AfterEffect->Transform()->SetWorldPos(Pos);

        if (ChangeHeight <= 0.f)
        {
            ChangeStep(StateStep::Progress);
        }
    }
}

void CElfilisG_NormalAtkTeleportFinishL::Progress()
{
    // resize Hitbox
    CBoxCollider* pHitbox = ELFFSM->GetHitbox();
    if (pHitbox && !GetOwner()->Animator()->IsChainging())
    {
        if (GetOwner()->Animator()->GetClipFrameIndex() > 32 && GetOwner()->Animator()->GetClipFrameIndex() < 42)
        {
            pHitbox->GetOwner()->SetActive(true);
            pHitbox->Transform()->SetLocalPos(Vec3(0.f, 1.f, 0.f));
            pHitbox->Transform()->SetLocalRotation(Vec3(0.f));
            pHitbox->Transform()->SetLocalScale(Vec3(7.5f, 1.f, 7.5f));
        }
        else
        {
            pHitbox->GetOwner()->SetActive(false);
        }
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisG_NormalAtkTeleportFinishL::End()
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

void CElfilisG_NormalAtkTeleportFinishL::SpawnTeleport()
{ //@Effect 일부분만 그리는 셰이더 작성 필요

    // copy object
    m_BeforeObj = new CGameObject;
    m_BeforeObj->AddComponent(GetOwner()->Transform()->Clone());
    m_BeforeObj->AddComponent(GetOwner()->MeshRender()->Clone());
    m_BeforeObj->AddComponent(GetOwner()->Animator()->Clone());
    m_BeforeObj->SetName(L"Effect_ElfilisTelport Body");
    GamePlayStatic::SpawnGameObject(m_BeforeObj, LAYER_MONSTER);

    // get teleport pos
    Vec3 Dist = GetOwner()->Transform()->GetWorldPos() - PLAYER->Transform()->GetWorldPos();
    Dist.y = 0.f;
    Dist.Normalize();
    Dist *= ELFFSM->GetNearDist() * (GetRandomfloat(30.f, 100.f) / 100.f);
    m_AfterPos = PLAYER->Transform()->GetWorldPos() + Dist;
    m_AfterPos.y = 0.f;

    // limit in map
    float MapSizeRadius = ELFFSM->GetMapSizeRadius();
    Vec3 MapFloorOffset = ELFFSM->GetMapFloorOffset();

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
