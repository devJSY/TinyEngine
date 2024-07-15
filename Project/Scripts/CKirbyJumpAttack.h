#pragma once
#include "CState.h"

class CKirbyJumpAttack : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyJumpAttack)
    CKirbyJumpAttack();
    virtual ~CKirbyJumpAttack();
};
