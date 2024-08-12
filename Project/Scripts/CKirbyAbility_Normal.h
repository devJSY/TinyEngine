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
    // πÒ±‚
    virtual void Attack();
    virtual void AttackEnter();
    virtual void AttackExit();

    // »Ì¿‘1
    virtual void AttackCharge1();
    virtual void AttackCharge1Enter();
    virtual void AttackCharge1Exit();

    virtual void AttackCharge1Start();
    virtual void AttackCharge1StartEnter();
    virtual void AttackCharge1StartExit();

    virtual void AttackCharge1End();
    virtual void AttackCharge1EndEnter();
    virtual void AttackCharge1EndExit();

    virtual void AttackCharge1Run();
    virtual void AttackCharge1RunEnter();
    virtual void AttackCharge1RunExit();

    // »Ì¿‘2
    virtual void AttackCharge2();
    virtual void AttackCharge2Enter();
    virtual void AttackCharge2Exit();

    virtual void AttackCharge2Start();
    virtual void AttackCharge2StartEnter();
    virtual void AttackCharge2StartExit();

    virtual void AttackCharge2Run();
    virtual void AttackCharge2RunEnter();
    virtual void AttackCharge2RunExit();

    // ∫ØΩ≈
    virtual void ChangeAbility();
    virtual void ChangeAbilityEnter();
    virtual void ChangeAbilityExit();

public:
    CLONE(CKirbyAbility_Normal)
    CKirbyAbility_Normal();
    CKirbyAbility_Normal(const CKirbyAbility_Normal& _Origin);
    virtual ~CKirbyAbility_Normal();
};
