#pragma once
#include "CUnitScript.h"

class CKirbyUnitScript : public CUnitScript
{
private:
    Ptr<CPrefab> m_AbilityBubble;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void AttackReward();
    void BuffHP(float _HP);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    virtual void OnControllerColliderHit(struct ControllerColliderHit Hit);
    void DropAbility();

public:
    CLONE(CKirbyUnitScript)
    CKirbyUnitScript();
    CKirbyUnitScript(const CKirbyUnitScript& _Origin);
    virtual ~CKirbyUnitScript();
};
