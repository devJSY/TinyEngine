#include "pch.h"
#include "CMonsterTest.h"
#include "CUnitScript.h"
#include "CPlayerMgr.h"
#include "CKirbyUnitScript.h"

void CMonsterTest::OnCollisionEnter(CCollider* _OtherCollider)
{
    int Layeridx = _OtherCollider->GetOwner()->GetLayerIdx();

    if (Layeridx == LAYER_PLAYER)
    {
        Vec3 HitDir = (Transform()->GetWorldPos() - _OtherCollider->Transform()->GetWorldPos()).Normalize();
        UnitHit HitInfo = {DAMAGE_TYPE::NORMAL, HitDir, 10.f, 0.f, 0.f};
        CUnitScript* pMonster = GetOwner()->GetScript<CUnitScript>();

        PLAYERUNIT->GetDamage(HitInfo);
        // pMonster->AttackReward();
    }
}

void CMonsterTest::OnCollisionStay(CCollider* _OtherCollider)
{
}

void CMonsterTest::OnCollisionExit(CCollider* _OtherCollider)
{
}

void CMonsterTest::OnTriggerEnter(CCollider* _OtherCollider)
{
    int Layeridx = _OtherCollider->GetOwner()->GetLayerIdx();
    
    if (Layeridx == LAYER_PLAYER)
    {
        Vec3 HitDir = _OtherCollider->Transform()->GetWorldPos() - Transform()->GetWorldPos();
        HitDir.y = 0.f;
        HitDir.Normalize();
        UnitHit HitInfo = {DAMAGE_TYPE::NORMAL, HitDir, 10.f, 0.f, 0.f};
        CUnitScript* pMonster = GetOwner()->GetScript<CUnitScript>();

        PLAYERUNIT->GetDamage(HitInfo);
        //pMonster->AttackReward();
    }
}

void CMonsterTest::OnTriggerStay(CCollider* _OtherCollider)
{
}

void CMonsterTest::OnTriggerExit(CCollider* _OtherCollider)
{
}

void CMonsterTest::SaveToLevelFile(FILE* _File)
{
}

void CMonsterTest::LoadFromLevelFile(FILE* _File)
{
}

CMonsterTest::CMonsterTest()
    : CScript(MONSTERTEST)
{
}

CMonsterTest::~CMonsterTest()
{
}
