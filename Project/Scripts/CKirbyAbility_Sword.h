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

    // 칼휘두르기 (Lv0)
    virtual void Attack();
    virtual void AttackEnter();
    virtual void AttackExit();

    // 칼휘두르기 (Lv1)
    virtual void AttackCombo1();
    virtual void AttackCombo1Enter();
    virtual void AttackCombo1Exit();

    // 칼휘두르기 (Lv2)
    virtual void AttackCombo2();
    virtual void AttackCombo2Enter();
    virtual void AttackCombo2Exit();

    // 스핀공격 (Lv1)
    virtual void AttackCharge1();
    virtual void AttackCharge1Enter();
    virtual void AttackCharge1Exit();

    virtual void AttackCharge1Start();
    virtual void AttackCharge1StartEnter();
    virtual void AttackCharge1StartExit();

    virtual void AttackCharge1End();
    virtual void AttackCharge1EndEnter();
    virtual void AttackCharge1EndExit();

    // 스핀공격 (Lv2)
    virtual void AttackCharge2();
    virtual void AttackCharge2Enter();
    virtual void AttackCharge2Exit();

    virtual void AttackCharge2Start();
    virtual void AttackCharge2StartEnter();
    virtual void AttackCharge2StartExit();

    // 대검찍기 (Lv3)
    virtual void AttackCharge3();
    virtual void AttackCharge3Enter();
    virtual void AttackCharge3Exit();

    virtual void AttackCharge3Start();
    virtual void AttackCharge3StartEnter();
    virtual void AttackCharge3StartExit();

    virtual void AttackCharge3End();
    virtual void AttackCharge3EndEnter();
    virtual void AttackCharge3EndExit();

    // 점프
    virtual void JumpFallEnter();

    // 점프공격
     virtual void JumpAttack();
     virtual void JumpAttackEnter();
     virtual void JumpAttackExit();

     virtual void JumpAttackStart();
     virtual void JumpAttackStartEnter();
     virtual void JumpAttackStartExit();

     // 랜딩
     virtual void LandingEnter();
     virtual void LandingExit();
     
     // 가드
     virtual void GuardEnter();

     virtual void GuardRun();
     virtual void GuardRunEnter();
     virtual void GuardRunExit();

     // 슬라이드 찌르기
     virtual void Slide();
     virtual void SlideEnter();
     virtual void SlideExit();

     virtual void SlideStart();
     virtual void SlideStartEnter();
     virtual void SlideStartExit();

     virtual void SlideEnd();
     virtual void SlideEndEnter();
     virtual void SlideEndExit();

     // 슬라이드 콤보공격
     virtual void SlideAttack();
     virtual void SlideAttackEnter();
     virtual void SlideAttackExit();

    // 변신 돌입
    virtual void ChangeAbility();
    virtual void ChangeAbilityEnter();
    virtual void ChangeAbilityExit();

public:
    CLONE(CKirbyAbility_Sword)
    CKirbyAbility_Sword();
    virtual ~CKirbyAbility_Sword();
};