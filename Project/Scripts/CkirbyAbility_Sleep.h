#pragma once
#include "CKirbyAbility.h"

class CkirbyAbility_Sleep : public CKirbyAbility
{
public:
    // 기본 동작 상태
    virtual void IdleEnter();
    virtual void IdleStartEnter();
    virtual void RunEnter();
    virtual void JumpFallEnter();

    // 잠들기
    virtual void AttackEnter();
    virtual void AttackStartEnter();
    virtual void AttackEndEnter();

public:
    CLONE_DISABLE(CkirbyAbility_Sleep)
    CkirbyAbility_Sleep();
    virtual ~CkirbyAbility_Sleep();
};
