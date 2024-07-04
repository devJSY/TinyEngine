#pragma once
#include "CState.h"
class CKirbyDodgeRight1 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDodgeRight1)
    CKirbyDodgeRight1();
    virtual ~CKirbyDodgeRight1();
};
