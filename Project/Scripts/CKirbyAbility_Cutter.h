#pragma once
#include "CKirbyAbility.h"
class CKirbyAbility_Cutter : public CKirbyAbility
{
private:
    Ptr<CPrefab> m_CutterBulletPrefab;
    bool m_IsNextCombo;

public:
    virtual void Attack() override;
    virtual void AttackEnter() override;
    virtual void AttackExit() override;

    virtual void AttackCombo1() override;
    virtual void AttackCombo1Enter() override;
    virtual void AttackCombo1Exit() override;

    virtual void AttackCombo2() override;
    virtual void AttackCombo2Enter() override;
    virtual void AttackCombo2Exit() override;

    virtual void ChangeAbility() override;
    virtual void ChangeAbilityEnter() override;
    virtual void ChangeAbilityExit() override;

public:
    CLONE_DISABLE(CKirbyAbility_Cutter)
    CKirbyAbility_Cutter();
    virtual ~CKirbyAbility_Cutter();

};
