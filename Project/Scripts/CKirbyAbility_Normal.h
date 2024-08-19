#pragma once
#include "CKirbyAbility.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

class CKirbyAbility_Normal : public CKirbyAbility
{
private:
    bool        m_bFrmEnter;
    float       m_SavedSpeed;

public:
    // ¹ñ±â
    virtual void Attack();
    virtual void AttackEnter();
    virtual void AttackExit();

    // º¯½Å
    virtual void ChangeAbility();
    virtual void ChangeAbilityEnter();
    virtual void ChangeAbilityExit();

public:
    CLONE(CKirbyAbility_Normal)
    CKirbyAbility_Normal();
    CKirbyAbility_Normal(const CKirbyAbility_Normal& _Origin);
    virtual ~CKirbyAbility_Normal();
};
