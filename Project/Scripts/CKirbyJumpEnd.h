#pragma once
#include "CState.h"

class CKirbyJumpEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyJumpEnd)
    CKirbyJumpEnd();
    virtual ~CKirbyJumpEnd();
};
