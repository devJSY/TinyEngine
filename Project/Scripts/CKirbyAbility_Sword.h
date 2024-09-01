#pragma once
#include "CKirbyAbility.h"

class CKirbyAbility_Sword : public CKirbyAbility
{
private:
    Ptr<CPrefab> m_KirbySwordSlashPref;
    Vec3 m_PrevWeaponScale;
    Vec3 m_BigWeaponScale;
    float m_PrevSpeed;
    float m_PrevRotSpeed;
    float m_PrevGravity;
    float m_AccTime;
    bool m_bFrmEnter;

public:
    // Idle
    virtual void IdleEnter() override;

    // Ä®ÈÖµÎ¸£±â (Lv0)
    virtual void Attack() override;
    virtual void AttackEnter() override;
    virtual void AttackExit() override;

    // Ä®ÈÖµÎ¸£±â (Lv1)
    virtual void AttackCombo1() override;
    virtual void AttackCombo1Enter() override;
    virtual void AttackCombo1Exit() override;

    // Ä®ÈÖµÎ¸£±â (Lv2)
    virtual void AttackCombo2() override;
    virtual void AttackCombo2Enter() override;
    virtual void AttackCombo2Exit() override;

    // ½ºÇÉ°ø°Ý (Lv1)
    virtual void AttackCharge1() override;
    virtual void AttackCharge1Enter() override;
    virtual void AttackCharge1Exit() override;

    virtual void AttackCharge1Start() override;
    virtual void AttackCharge1StartEnter() override;
    virtual void AttackCharge1StartExit() override;

    virtual void AttackCharge1End() override;
    virtual void AttackCharge1EndEnter() override;
    virtual void AttackCharge1EndExit() override;

    // ½ºÇÉ°ø°Ý (Lv2)
    virtual void AttackCharge2() override;
    virtual void AttackCharge2Enter() override;
    virtual void AttackCharge2Exit() override;

    virtual void AttackCharge2Start() override;
    virtual void AttackCharge2StartEnter() override;
    virtual void AttackCharge2StartExit() override;

    // ´ë°ËÂï±â (Lv3)
    virtual void AttackCharge3() override;
    virtual void AttackCharge3Enter() override;
    virtual void AttackCharge3Exit() override;

    virtual void AttackCharge3Start() override;
    virtual void AttackCharge3StartEnter() override;
    virtual void AttackCharge3StartExit() override;

    virtual void AttackCharge3End() override;
    virtual void AttackCharge3EndEnter() override;
    virtual void AttackCharge3EndExit() override;

    // Á¡ÇÁ
    virtual void JumpFallEnter() override;

    // Á¡ÇÁ°ø°Ý
    virtual void JumpAttack() override;
    virtual void JumpAttackEnter() override;
    virtual void JumpAttackExit() override;

    virtual void JumpAttackStart() override;
    virtual void JumpAttackStartEnter() override;
    virtual void JumpAttackStartExit() override;

    // ·£µù
    virtual void LandingEnter() override;
    virtual void LandingExit() override;

    // °¡µå
    virtual void GuardEnter() override;

    virtual void GuardRun() override;
    virtual void GuardRunEnter() override;
    virtual void GuardRunExit() override;

    // ½½¶óÀÌµå Âî¸£±â
    virtual void Slide() override;
    virtual void SlideEnter() override;
    virtual void SlideExit() override;

    virtual void SlideStart() override;
    virtual void SlideStartEnter() override;
    virtual void SlideStartExit() override;

    virtual void SlideEnd() override;
    virtual void SlideEndEnter() override;
    virtual void SlideEndExit() override;

    // ½½¶óÀÌµå ÄÞº¸°ø°Ý
    virtual void SlideAttack() override;
    virtual void SlideAttackEnter() override;
    virtual void SlideAttackExit() override;

    // º¯½Å µ¹ÀÔ
    virtual void ChangeAbility() override;
    virtual void ChangeAbilityEnter() override;
    virtual void ChangeAbilityExit() override;

private:
    void SpawnSwordSlash(Vec3 _SlashScale, bool _bVertical = false);

public:
    CLONE(CKirbyAbility_Sword)
    CKirbyAbility_Sword();
    CKirbyAbility_Sword(const CKirbyAbility_Sword& _Origin);
    virtual ~CKirbyAbility_Sword();
};