#pragma once
#include "CKirbyAbility.h"

class CKirbyAbility_Sword : public CKirbyAbility
{
private:
    bool    m_bFrmEnter;

public:
    // Ä®ÈÖµÎ¸£±â (Lv0)
    virtual void Attack();
    virtual void AttackEnter();
    virtual void AttackExit();

    virtual void AttackEnd();
    virtual void AttackEndEnter();
    virtual void AttackEndExit();

    // Ä®ÈÖµÎ¸£±â (Lv1)
    virtual void AttackCombo1();
    virtual void AttackCombo1Enter();
    virtual void AttackCombo1Exit();

    // Ä®ÈÖµÎ¸£±â (Lv2)
    virtual void AttackCombo2();
    virtual void AttackCombo2Enter();
    virtual void AttackCombo2Exit();

    virtual void AttackCombo2End();
    virtual void AttackCombo2EndEnter();
    virtual void AttackCombo2EndExit();

    // 
    //virtual void AttackCharge1();
    //virtual void AttackCharge1Enter();
    //virtual void AttackCharge1Exit();

    //virtual void AttackCharge1Start();
    //virtual void AttackCharge1StartEnter();
    //virtual void AttackCharge1StartExit();

    //virtual void AttackCharge1Run();
    //virtual void AttackCharge1RunEnter();
    //virtual void AttackCharge1RunExit();

    //// Á¡ÇÁ°ø°Ý
    //virtual void JumpAttack();
    //virtual void JumpAttackEnter();
    //virtual void JumpAttackExit();

public:
    CLONE_DISABLE(CKirbyAbility_Sword)
    CKirbyAbility_Sword();
    virtual ~CKirbyAbility_Sword();
};