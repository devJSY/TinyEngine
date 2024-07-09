#include "pch.h"
#include "CKirbyVacuumCollider.h"
#include "CPlayerMgr.h"
#include "CMonsterUnitScript.h"
#include "CKirbyCopyAbilityScript.h"
#include "CKirbyCopyObjScript.h"

CKirbyVacuumCollider::CKirbyVacuumCollider()
    : CScript(KIRBYVACUUMCOLLIDER)
    , m_FindTarget(nullptr)
    , m_FindDistance(0.f)
    , m_FindType(EatType::NONE)
{
}

CKirbyVacuumCollider::CKirbyVacuumCollider(const CKirbyVacuumCollider& _Origin)
    : CScript(KIRBYVACUUMCOLLIDER)
    , m_FindTarget(nullptr)
    , m_FindDistance(0.f)
    , m_FindType(EatType::NONE)
{
}

CKirbyVacuumCollider::~CKirbyVacuumCollider()
{
}

void CKirbyVacuumCollider::tick()
{
}

void CKirbyVacuumCollider::OnTriggerEnter(CCollider* _OtherCollider)
{
    // 흡수할 수 있는 물체라면
    // if (!_OtherCollider->GetOwner()->IsTagged(L"Eatable"))
    //    return;

    bool bChanged = false;
    AbilityCopyType newAbility = AbilityCopyType::NONE;
    ObjectCopyType newObject = ObjectCopyType::NONE;
    EatType newType = GetEatType(_OtherCollider->GetOwner(), newAbility, newObject);
    float newDist = (PLAYER->Transform()->GetWorldPos() - _OtherCollider->Transform()->GetWorldPos()).Length();

    // 현재 흡수예정인 오브젝트와 비교해 우선순위 판단
    if ((UINT)newType < (UINT)m_FindType)
    {
        bChanged = true;
    }
    else if ((UINT)newType == (UINT)m_FindType)
    {
        if (newDist < m_FindDistance)
        {
            bChanged = true;
        }
    }

    if (bChanged)
    {
        m_FindTarget = _OtherCollider->GetOwner();
        m_FindDistance = newDist;

        // change player state
        switch (m_FindType)
        {
        case EatType::Copy_Ability:
        case EatType::Copy_Monster:
            PLAYERFSM->ChangeAbilityCopy(newAbility);
            break;
        case EatType::Copy_Object:
            PLAYERFSM->ChangeObjectCopy(newObject);
            break;
        case EatType::Monster:
        case EatType::Etc:
            PLAYERFSM->StartStuffed(m_FindTarget);
            break;
        }
    }
}

EatType CKirbyVacuumCollider::GetEatType(CGameObject* _pObj, AbilityCopyType& _outAbility, ObjectCopyType& _outObj)
{
    if (_pObj->GetScript<CKirbyCopyAbilityScript>())
    {
        _outAbility = _pObj->GetScript<CKirbyCopyAbilityScript>()->GetAbilityType();
        return EatType::Copy_Ability;
    }
    else if (_pObj->GetScript<CMonsterUnitScript>())
    {
        CMonsterUnitScript* pMonster = _pObj->GetScript<CMonsterUnitScript>();
        if (pMonster->GetAbilityType() != AbilityCopyType::NONE)
        {
            _outAbility = pMonster->GetAbilityType();
            return EatType::Copy_Monster;
        }
        else
        {
            return EatType::Monster;
        }
    }
    else if (_pObj->GetScript<CKirbyCopyObjScript>())
    {
        _outObj = _pObj->GetScript<CKirbyCopyObjScript>()->GetObjType();
        return EatType::Copy_Object;
    }
    else
    {
        return EatType::Etc;
    }
}

void CKirbyVacuumCollider::SaveToLevelFile(FILE* _File)
{
}

void CKirbyVacuumCollider::LoadFromLevelFile(FILE* _File)
{
}