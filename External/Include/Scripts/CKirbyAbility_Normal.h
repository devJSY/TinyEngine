#pragma once
#include "CKirbyAbility.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

class CKirbyAbility_Normal : public CKirbyAbility
{
private:
    bool        m_bFrmEnter;
    const float m_Charge1Time;

public:
    // RUN
    virtual void Run();
    virtual void RunEnter();
    virtual void RunExit();

    virtual void RunStart();
    virtual void RunStartEnter();
    virtual void RunStartExit();

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

    // »Ì¿‘2
    virtual void AttackCharge2();
    virtual void AttackCharge2Enter();
    virtual void AttackCharge2Exit();

    virtual void JumpAttack(){};
    virtual void JumpAttackEnter(){};
    virtual void JumpAttackExit(){};

public:
    CLONE_DISABLE(CKirbyAbility_Normal)
    CKirbyAbility_Normal();
    virtual ~CKirbyAbility_Normal();
};
