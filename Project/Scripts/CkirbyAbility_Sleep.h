#pragma once
#include "CKirbyAbility.h"

class CkirbyAbility_Sleep : public CKirbyAbility
{
private:
    CGameObject* m_SnotBubble;
    float m_PrevSpeed;
    const float m_Speed;

public:
    // 기본 동작 상태
    virtual void IdleEnter();

    virtual void IdleStartEnter();
    virtual void IdleStartExit();

    virtual void RunEnter();
    virtual void RunExit();

    virtual void JumpFallEnter();
    virtual void JumpFallExit();

    // 잠들기
    virtual void Attack();
    virtual void AttackEnter();
    virtual void AttackExit();

    virtual void AttackStartEnter();
    virtual void AttackStartExit();

    virtual void AttackEndEnter();
    virtual void AttackEndExit();

    // 변신 돌입
    virtual void ChangeAbility();
    virtual void ChangeAbilityEnter();
    virtual void ChangeAbilityExit();

public:
    CLONE(CkirbyAbility_Sleep)
    CkirbyAbility_Sleep();
    virtual ~CkirbyAbility_Sleep();
};
