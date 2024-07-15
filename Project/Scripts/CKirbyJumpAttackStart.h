#pragma once
#include "CState.h"

class CKirbyJumpAttackStart : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyJumpAttackStart)
    CKirbyJumpAttackStart();
    virtual ~CKirbyJumpAttackStart();
};
