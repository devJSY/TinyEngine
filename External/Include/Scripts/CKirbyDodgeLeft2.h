#pragma once
#include "CState.h"
class CKirbyDodgeLeft2 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDodgeLeft2)
    CKirbyDodgeLeft2();
    virtual ~CKirbyDodgeLeft2();
};
