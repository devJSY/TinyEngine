#pragma once
#include "CKirbyAbility.h"
class CKirbyAbility_Fire : public CKirbyAbility
{
public:
    virtual void Attack() override;
    virtual void AttackEnter() override;
    virtual void AttackExit() override;

    virtual void JumpAttack() override;
    virtual void JumpAttackEnter() override;
    virtual void JumpAttackExit() override;

    virtual void ChangeAbility() override;
    virtual void ChangeAbilityEnter() override;
    virtual void ChangeAbilityExit() override;

public:
    CLONE_DISABLE(CKirbyAbility_Fire)
    CKirbyAbility_Fire();
    virtual ~CKirbyAbility_Fire();
};
