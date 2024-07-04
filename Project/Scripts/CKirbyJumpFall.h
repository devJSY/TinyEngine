#pragma once
#include "CState.h"

class CKirbyJumpFall : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyJumpFall)
    CKirbyJumpFall();
    virtual ~CKirbyJumpFall();
};
