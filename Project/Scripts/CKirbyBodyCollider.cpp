#include "pch.h"
#include "CKirbyBodyCollider.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CKirbyVacuumCollider.h"
#include "CKirbyUnitScript.h"
#include "CState.h"

#include "CState.h"

CKirbyBodyCollider::CKirbyBodyCollider()
    : CScript(KIRBYBODYCOLLIDER)
{
}

CKirbyBodyCollider::~CKirbyBodyCollider()
{
}

void CKirbyBodyCollider::begin()
{
    // Level이 시작되면 BodyCollider의 크기를 세팅한다.

    Transform()->SetAbsolute(false);
    Transform()->SetLocalScale(Vec3(1.2f, 1.2f, 1.2f));
    CapsuleCollider()->SetTrigger(true);
    CapsuleCollider()->SetCenter(Vec3(0.f, 0.65f, 0.f));
    CapsuleCollider()->SetHeight(1.51f);
    CapsuleCollider()->SetRadius(0.51f);
}

void CKirbyBodyCollider::OnTriggerEnter(CCollider* _OtherCollider)
{
    int LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();

    // 흡입을 시작한 상태(타겟을 결정해 빨아들이기 시작한 상태)에서 충돌한 경우
    if (PLAYERFSM->IsDrawing())
    {
        PLAYERFSM->DrawingCollisionEnter(_OtherCollider->GetOwner());
        return;
    }

    // monster : 데미지 가함
    if (LayerIdx == LAYER_MONSTER)
    {
        // Player가 Dodge 중이라면 몬스터에게 데미지를 주지 않는다.
        if (PLAYERFSM->GetCurState()->GetName() == L"DODGE_START" || PLAYERFSM->GetCurState()->GetName() == L"DODGE1" ||
            PLAYERFSM->GetCurState()->GetName() == L"DODGE2")
            return;

        Vec3 HitDir = (_OtherCollider->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
        float HitDamage = FindDamage();
        UnitHit HitInfo = {DAMAGE_TYPE::NORMAL, HitDir, HitDamage, 0.f, 0.f};
        CUnitScript* pMonster = _OtherCollider->GetOwner()->GetScript<CUnitScript>();

        if (!pMonster)
            return;
        pMonster->GetDamage(HitInfo);
        ((CUnitScript*)PLAYERUNIT)->AttackReward();
    }
}

void CKirbyBodyCollider::OnTriggerStay(CCollider* _OtherCollider)
{
    if (PLAYERFSM->IsDrawing())
    {
        PLAYERFSM->DrawingCollisionEnter(_OtherCollider->GetOwner());
        return;
    }
}

float CKirbyBodyCollider::FindDamage()
{
    float Damage = 5.f;
    wstring StateName = PLAYERFSM->GetCurState()->GetName();

    if (PLAYERFSM->GetCurObject())
    {
        switch (PLAYERFSM->GetCurObjectIdx())
        {
        case ObjectCopyType::CONE:
            if (StateName == L"ATTACK" || StateName == L"ATTACK_END" || L"ATTACK_COMBO1")
            {
                Damage = 100.f;
            }
            break;
        }
    }

    return Damage;
}

UINT CKirbyBodyCollider::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CKirbyBodyCollider::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}
