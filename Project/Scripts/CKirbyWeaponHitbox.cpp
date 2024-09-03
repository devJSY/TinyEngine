#include "pch.h"
#include "CKirbyWeaponHitbox.h"
#include "CPlayerMgr.h"
#include "CUnitScript.h"
#include "CKirbyFSM.h"
#include "CState.h"
#include "CCameraController.h"

CKirbyWeaponHitbox::CKirbyWeaponHitbox()
    : CScript(KIRBYWEAPONHITBOX)
{
}

CKirbyWeaponHitbox::~CKirbyWeaponHitbox()
{
}

void CKirbyWeaponHitbox::OnTriggerEnter(CCollider* _OtherCollider)
{
    int LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();

    // monster : 데미지 가함
    if (LayerIdx == LAYER_MONSTER)
    {
        CUnitScript* pMonster = _OtherCollider->GetOwner()->GetScript<CUnitScript>();
        if (!pMonster)
            return;

        Vec3 HitDir = (_OtherCollider->Transform()->GetWorldPos() - PLAYER->Transform()->GetWorldPos()).Normalize();
        UnitHit HitInfo = {DAMAGE_TYPE::NORMAL, HitDir, 5.f, 0.f, 0.f};
        HitInfo.Damage = LoadDamage();
        ApplyEffect();

        SlashEffect(_OtherCollider->Transform()->GetWorldPos());

        pMonster->GetDamage(HitInfo);
        ((CUnitScript*)PLAYERUNIT)->AttackReward();
    }
}

float CKirbyWeaponHitbox::LoadDamage()
{
    float damage = 5.f;
    wstring state = PLAYERFSM->GetCurState()->GetName();

    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::FIRE:
        break;
    case AbilityCopyType::CUTTER:
        break;
    case AbilityCopyType::SWORD: {
        if (state.find(L"CHARGE1") != wstring::npos)
        {
            damage = 7.5f;
        }
        else if (state.find(L"CHARGE2") != wstring::npos)
        {
            damage = 10.f;
        }
        else if (state.find(L"CHARGE3") != wstring::npos)
        {
            damage = 20.f;
        }
        else if (state.find(L"COMBO2") != wstring::npos)
        {
            damage = 7.5f;
        }
        else if (state.find(L"GUARD_RUN") != wstring::npos)
        {
            damage = 10.f;
        }
        else if (state.find(L"JUMP_ATTACK") != wstring::npos)
        {
            damage = 10.f;
        }
        else if (state.find(L"JUMP_ATTACK") != wstring::npos)
        {
            damage = 10.f;
        }
        else if (state.find(L"SLIDE_ATTACK") != wstring::npos)
        {
            damage = 5.f;
        }
        else if (state.find(L"SLIDE") != wstring::npos)
        {
            damage = 7.5f;
        }
    }
    break;
    case AbilityCopyType::SLEEP:
        break;
    }

    return damage;
}

void CKirbyWeaponHitbox::ApplyEffect()
{
    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::FIRE:
        break;
    case AbilityCopyType::CUTTER:
        break;
    case AbilityCopyType::SWORD: {
        CAMERACTRL->Shake(0.1f, 20.f, 20.f);
    }
    break;
    case AbilityCopyType::SLEEP:
        break;
    }
}

void CKirbyWeaponHitbox::SlashEffect(Vec3 _vPos)
{
    wstring state = PLAYERFSM->GetCurState()->GetName();

    SpawnSlashEffect(_vPos, state.find(L"COMBO2") != wstring::npos ? 2 : 1);
}

void CKirbyWeaponHitbox::SpawnSlashEffect(Vec3 _vPos, int _iCount)
{
    for (int i = 0; i < _iCount; i++)
    {
        CGameObject* pSpawnEffect =
            CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_SlashEffect.pref", L"prefab\\Effect_SlashEffect.pref")->Instantiate();

        Vec3 vPos = _vPos;
        vPos.y += 35.f;
        pSpawnEffect->Transform()->SetWorldPos(vPos);
        GamePlayStatic::SpawnGameObject(pSpawnEffect, pSpawnEffect->GetLayerIdx());
    }
}

UINT CKirbyWeaponHitbox::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CKirbyWeaponHitbox::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}
