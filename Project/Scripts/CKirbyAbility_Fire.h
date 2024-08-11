#pragma once
#include "CKirbyAbility.h"
class CKirbyAbility_Fire : public CKirbyAbility
{
private:
    bool m_bFrmEnter;
    float m_SavedSpeed;

public:
    virtual void AttackCharge1Start() override;
    virtual void AttackCharge1StartEnter() override;
    virtual void AttackCharge1StartExit() override;

    virtual void AttackCharge1() override;
    virtual void AttackCharge1Enter() override;
    virtual void AttackCharge1Exit() override;

    virtual void AttackCharge1End() override;
    virtual void AttackCharge1EndEnter() override;
    virtual void AttackCharge1EndExit() override;

    virtual void AttackCharge1Run() override;
    virtual void AttackCharge1RunEnter() override;
    virtual void AttackCharge1RunExit() override;

    virtual void JumpAttack() override;
    virtual void JumpAttackEnter() override;
    virtual void JumpAttackExit() override;

    virtual void ChangeAbility() override;
    virtual void ChangeAbilityEnter() override;
    virtual void ChangeAbilityExit() override;

public:
    CLONE(CKirbyAbility_Fire)
    CKirbyAbility_Fire();
    CKirbyAbility_Fire(const CKirbyAbility_Fire& _Origin);
    virtual ~CKirbyAbility_Fire();
};
