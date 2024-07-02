#pragma once
#include "CState.h"

class CKirbyJump : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyJump)
    CKirbyJump();
    virtual ~CKirbyJump();
};