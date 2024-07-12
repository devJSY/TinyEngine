#pragma once
#include "CKirbyAbility.h"

class CKirbyAbility_Sword : public CKirbyAbility
{
private:
    float   m_ComboTime;
    bool    m_bFrmEnter;

public:
    // Ä®ÈÖµÎ¸£±â (Lv1)
    virtual void Attack();
    virtual void AttackEnter();
    virtual void AttackExit();

    virtual void AttackEnd();
    virtual void AttackEndEnter();
    virtual void AttackEndExit();

    // 
    //virtual void AttackCharge2();
    //virtual void AttackCharge2Enter();
    //virtual void AttackCharge2Exit();

    //virtual void AttackCharge2Start();
    //virtual void AttackCharge2StartEnter();
    //virtual void AttackCharge2StartExit();

    //virtual void AttackCharge2Run();
    //virtual void AttackCharge2RunEnter();
    //virtual void AttackCharge2RunExit();

    //// Á¡ÇÁ°ø°Ý
    //virtual void JumpAttack();
    //virtual void JumpAttackEnter();
    //virtual void JumpAttackExit();

public:
    CLONE_DISABLE(CKirbyAbility_Sword)
    CKirbyAbility_Sword();
    virtual ~CKirbyAbility_Sword();
};