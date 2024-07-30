#include "pch.h"
#include "CKirbyWeaponHitbox.h"
#include "CPlayerMgr.h"
#include "CUnitScript.h"
#include "CKirbyFSM.h"
#include "CState.h"

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
    case AbilityCopyType::SWORD:
    {
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
