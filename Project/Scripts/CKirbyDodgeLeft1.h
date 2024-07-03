#pragma once
#include "CState.h"
class CKirbyDodgeLeft1 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDodgeLeft1)
    CKirbyDodgeLeft1();
    virtual ~CKirbyDodgeLeft1();
};
