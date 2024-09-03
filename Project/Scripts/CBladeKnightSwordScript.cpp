#include "pch.h"
#include "CBladeKnightSwordScript.h"
#include "CPlayerMgr.h"
#include "CKirbyUnitScript.h"

CBladeKnightSwordScript::CBladeKnightSwordScript()
    : CScript(BLADEKNIGHTSWORDSCRIPT)
    , m_State(BLADEKNIGHTSWORD_STATE::Thrust)
{
}

CBladeKnightSwordScript::CBladeKnightSwordScript(const CBladeKnightSwordScript& origin)
    : CScript(origin)
    , m_State(origin.m_State)
{
}

CBladeKnightSwordScript::~CBladeKnightSwordScript()
{
}

void CBladeKnightSwordScript::begin()
{
}

void CBladeKnightSwordScript::tick()
{
    // FSM
    switch (m_State)
    {
    case BLADEKNIGHTSWORD_STATE::Thrust:
        Thrust();
        break;
    case BLADEKNIGHTSWORD_STATE::ThrustEnd:
        ThrustEnd();
        break;
    case BLADEKNIGHTSWORD_STATE::ThrustLoop:
        ThrustLoop();
        break;
    case BLADEKNIGHTSWORD_STATE::ThrustStart:
        ThrustStart();
        break;
    case BLADEKNIGHTSWORD_STATE::ThrustStartWait:
        ThrustStartWait();
        break;
        // case BLADEKNIGHTSWORD_STATE::ThrustWait:
        //     ThrustWait();
        //     break;
    }
}

void CBladeKnightSwordScript::ChangeState(BLADEKNIGHTSWORD_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CBladeKnightSwordScript::EnterState()
{
    switch (m_State)
    {
    case BLADEKNIGHTSWORD_STATE::Thrust: {
        Animator()->Play(ANIMPREFIX("Thrust"));
    }
    break;
    case BLADEKNIGHTSWORD_STATE::ThrustEnd: {
        Animator()->Play(ANIMPREFIX("ThrustEnd"));
    }
    break;
    case BLADEKNIGHTSWORD_STATE::ThrustLoop: {
        Animator()->Play(ANIMPREFIX("ThrustLoop"), true, false, 1.f, 0.);
    }
    break;
    case BLADEKNIGHTSWORD_STATE::ThrustStart: {
        Animator()->Play(ANIMPREFIX("ThrustStart"));
    }
    break;
    case BLADEKNIGHTSWORD_STATE::ThrustStartWait: {
        Animator()->Play(ANIMPREFIX("ThrustStartWait"));
    }
    break;
        // case BLADEKNIGHTSWORD_STATE::ThrustWait: {
        // }
        // break;
    }
}

void CBladeKnightSwordScript::ExitState()
{
    switch (m_State)
    {
    case BLADEKNIGHTSWORD_STATE::Thrust: {
    }
    break;
    case BLADEKNIGHTSWORD_STATE::ThrustEnd: {
    }
    break;
    case BLADEKNIGHTSWORD_STATE::ThrustLoop: {
    }
    break;
    case BLADEKNIGHTSWORD_STATE::ThrustStart: {
    }
    break;
    case BLADEKNIGHTSWORD_STATE::ThrustStartWait: {
    }
    break;
        // case BLADEKNIGHTSWORD_STATE::ThrustWait: {
        // }
        // break;
    }
}

void CBladeKnightSwordScript::Thrust()
{
}

void CBladeKnightSwordScript::ThrustEnd()
{
}

void CBladeKnightSwordScript::ThrustLoop()
{
}

void CBladeKnightSwordScript::ThrustStart()
{
}

void CBladeKnightSwordScript::ThrustStartWait()
{
}

void CBladeKnightSwordScript::ThrustWait()
{
}

void CBladeKnightSwordScript::SlashEffect(Vec3 _vPos)
{
    CGameObject* pSpawnEffect =
        CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_SlashEffect.pref", L"prefab\\Effect_SlashEffect.pref")->Instantiate();

    Vec3 vPos = _vPos;
    vPos.y += 35.f;
    pSpawnEffect->Transform()->SetWorldPos(vPos);
    GamePlayStatic::SpawnGameObject(pSpawnEffect, pSpawnEffect->GetLayerIdx());
}

void CBladeKnightSwordScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    int Layeridx = _OtherCollider->GetOwner()->GetLayerIdx();

    if (Layeridx == LAYER_PLAYER)
    {
        Vec3 HitDir = _OtherCollider->Transform()->GetWorldPos() - Transform()->GetWorldPos();
        HitDir.Normalize();
        UnitHit HitInfo = {DAMAGE_TYPE::NORMAL, HitDir, 10.f, 0.f, 0.f};

        SlashEffect(_OtherCollider->GetOwner()->Transform()->GetWorldPos());

        PLAYERUNIT->GetDamage(HitInfo);
    }
}

UINT CBladeKnightSwordScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CBladeKnightSwordScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}
