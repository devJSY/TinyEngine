#pragma once
#include <Engine/CScript.h>
#include "CKirbyFSM.h"

enum class EatType
{
    Copy_Ability,
    Copy_Monster,
    Monster,
    Copy_Object,
    Etc,
    NONE,
};

class CKirbyVacuumCollider : public CScript
{
private:
    CGameObject*    m_FindTarget;
    float           m_FindDistance;
    EatType         m_FindType;

public:
    virtual void tick();

    virtual void OnTriggerEnter(CCollider* _OtherCollider);

private:
    EatType GetEatType(CGameObject* _pObj, AbilityCopyType& _outAbility, ObjectCopyType& _outObj);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyVacuumCollider);
    CKirbyVacuumCollider();
    CKirbyVacuumCollider(const CKirbyVacuumCollider& _Origin);
    virtual ~CKirbyVacuumCollider();
};