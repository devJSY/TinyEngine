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
    EatType         m_FindType;
    AbilityCopyType m_FindAbilityType;
    ObjectCopyType  m_FindObjType;
    float           m_FindDistance;
    float           m_FindHoldAccTime;
    float           m_FindHoldTime;
    float           m_DrawingAccTime;
    float           m_DrawingTime;
    bool            m_bDrawing;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;

    void DrawingCollisionEnter(CGameObject* _CollisionObject);
    void EnableCollider(bool _bEnable);

public:
    bool IsDrawing() { return m_bDrawing; }

private:
    EatType GetEatType(CGameObject* _pObj, AbilityCopyType& _outAbility, ObjectCopyType& _outObj, float& _outHoldTime);
    void CheckDrawing();
    void DrawingTarget();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyVacuumCollider);
    CKirbyVacuumCollider();
    CKirbyVacuumCollider(const CKirbyVacuumCollider& _Origin);
    virtual ~CKirbyVacuumCollider();
};