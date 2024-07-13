#pragma once
#include "CKirbyAbility.h"

class CKirbyAbility_Sword : public CKirbyAbility
{
private:
    float   m_PrevSpeed;
    float   m_PrevRotSpeed;
    float   m_PrevGravity;
    bool    m_bFrmEnter;

public:
    // Idle
    virtual void IdleEnter();

    // Ä®ÈÖµÎ¸£±â (Lv0)
    virtual void Attack();
    virtual void AttackEnter();
    virtual void AttackExit();

    // Ä®ÈÖµÎ¸£±â (Lv1)
    virtual void AttackCombo1();
    virtual void AttackCombo1Enter();
    virtual void AttackCombo1Exit();

    // Ä®ÈÖµÎ¸£±â (Lv2)
    virtual void AttackCombo2();
    virtual void AttackCombo2Enter();
    virtual void AttackCombo2Exit();

    // ½ºÇÉ°ø°Ý (Lv1)
    virtual void AttackCharge1();
    virtual void AttackCharge1Enter();
    virtual void AttackCharge1Exit();

    virtual void AttackCharge1Start();
    virtual void AttackCharge1StartEnter();
    virtual void AttackCharge1StartExit();

    virtual void AttackCharge1End();
    virtual void AttackCharge1EndEnter();
    virtual void AttackCharge1EndExit();

    // ½ºÇÉ°ø°Ý (Lv2)
    virtual void AttackCharge2();
    virtual void AttackCharge2Enter();
    virtual void AttackCharge2Exit();

    virtual void AttackCharge2Start();
    virtual void AttackCharge2StartEnter();
    virtual void AttackCharge2StartExit();

    // ´ë°ËÂï±â (Lv3)
    virtual void AttackCharge3();
    virtual void AttackCharge3Enter();
    virtual void AttackCharge3Exit();

    virtual void AttackCharge3Start();
    virtual void AttackCharge3StartEnter();
    virtual void AttackCharge3StartExit();

    virtual void AttackCharge3End();
    virtual void AttackCharge3EndEnter();
    virtual void AttackCharge3EndExit();

    // Á¡ÇÁ°ø°Ý
     virtual void JumpAttack();
     virtual void JumpAttackEnter();
     virtual void JumpAttackExit();

     virtual void JumpAttackStart();
     virtual void JumpAttackStartEnter();
     virtual void JumpAttackStartExit();

     // ½½¶óÀÌµå Âî¸£±â
     virtual void Slide();
     virtual void SlideEnter();
     virtual void SlideExit();

     virtual void SlideStart();
     virtual void SlideStartEnter();
     virtual void SlideStartExit();

     virtual void SlideEnd();
     virtual void SlideEndEnter();
     virtual void SlideEndExit();

    // º¯½Å µ¹ÀÔ
    virtual void ChangeAbility();
    virtual void ChangeAbilityEnter();
    virtual void ChangeAbilityExit();

public:
    CLONE_DISABLE(CKirbyAbility_Sword)
    CKirbyAbility_Sword();
    virtual ~CKirbyAbility_Sword();
};