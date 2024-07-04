#pragma once
#include "CState.h"
class CKirbyDodgeRight2 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDodgeRight2)
    CKirbyDodgeRight2();
    virtual ~CKirbyDodgeRight2();
};
