#include "pch.h"
#include "CElfilisG_NormalAtkTeleportR.h"
#include "CElfilisFSM.h"

CElfilisG_NormalAtkTeleportR::CElfilisG_NormalAtkTeleportR()
    : m_BeforeObj(nullptr)
    , m_BeforeEffect(nullptr)
    , m_AfterEffect(nullptr)
    , m_EffectSpeed(700.f)
    , m_bComboSuccess(false)
    , m_bFrmEnter(true)
{
    m_Effect = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Effect_ElfilisTeleport.pref");
}

CElfilisG_NormalAtkTeleportR::~CElfilisG_NormalAtkTeleportR()
{
}

void CElfilisG_NormalAtkTeleportR::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Wait: {
        StartWait();
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

void CElfilisG_NormalAtkTeleportR::Enter()
{
    if (ELFFSM->GetComboLevel() == 0)
    {
        m_Step = StateStep::Start;
    }
    else
    {
        m_Step = StateStep::Wait;
    }

    Enter_Step();
}

void CElfilisG_NormalAtkTeleportR::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRightStart"), false, false, 2.5f, 0.5f);
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ChainRL"), false, false, 1.5f, 0.5f);
    }
    break;
    case StateStep::StartEnd: {
        SpawnTeleport();
        m_bFrmEnter = true;
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRight"), false);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRightEnd"), false, false, 1.5f);
    }
    break;
    }
}

void CElfilisG_NormalAtkTeleportR::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Wait:
        break;
    case StateStep::StartEnd: {
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
        if (m_bComboSuccess)
        {
            ELFFSM->AddComboLevel();
        }
        else
        {
            ELFFSM->SetComboLevel(0);
        }
    }
    break;
    case StateStep::End:
        break;
    }
}

void CElfilisG_NormalAtkTeleportR::Start()
{
    // rotate
    RotateToPlayer();

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CElfilisG_NormalAtkTeleportR::StartWait()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        float Rand = GetRandomfloat(1.f, 100.f);
        
        if (Rand <= 80.f)
        {
            ChangeStep(StateStep::Progress);
        }
        else
        {
            ChangeStep(StateStep::StartEnd);
        }
    }
}

void CElfilisG_NormalAtkTeleportR::StartEnd()
{
    if (m_bFrmEnter)
    {
        // Teleport (After 1 tick : Spawn 생성 기다림)
        Vec3 Dir = PLAYER->Transform()->GetWorldPos() - m_AfterPos;
        Dir.y = 0.f;
        Dir.Normalize();

        GetOwner()->Transform()->Slerp(Dir, 1.f);
        GetOwner()->Transform()->SetWorldPos(m_AfterPos);
        GetOwner()->Rigidbody()->SetKinematic(true);

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

void CElfilisG_NormalAtkTeleportR::Progress()
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

            if (ELFFSM->GetPhase() == 1 || (ELFFSM->GetPhase() == 2) && ELFFSM->GetComboLevel() == 2)
            {
                ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundMoveAtk, L"GROUND_MOVEATK_NORMALTELEPORT_FINISHL");
            }
            else
            {
                ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundMoveAtk, L"GROUND_MOVEATK_NORMALTELEPORT_L");
            }
        }
        else
        {
            ChangeStep(StateStep::End);
        }
    }
}

void CElfilisG_NormalAtkTeleportR::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();
        ELFFSM->ChangeStateGroup_RandState(NextState);
    }
}

void CElfilisG_NormalAtkTeleportR::SpawnTeleport()
{
    GetOwner()->Rigidbody()->SetKinematic(true);
    //@Effect 일부분만 그리는 셰이더 작성 필요

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
    m_AfterPos = GetOwner()->Transform()->GetWorldPos() + Dist;

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
