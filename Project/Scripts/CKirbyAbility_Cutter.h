#pragma once
#include "CKirbyAbility.h"
class CKirbyAbility_Cutter : public CKirbyAbility
{
private:
    Ptr<CPrefab> m_CutterBulletPrefab;

public:
    virtual void Attack() override;
    virtual void AttackEnter() override;
    virtual void AttackExit() override;

    virtual void ChangeAbility() override;
    virtual void ChangeAbilityEnter() override;
    virtual void ChangeAbilityExit() override;

public:
    CLONE_DISABLE(CKirbyAbility_Cutter)
    CKirbyAbility_Cutter();
    virtual ~CKirbyAbility_Cutter();

};
