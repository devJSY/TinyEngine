#pragma once
#include "CState.h"
class CKirbyDodgeBack1 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDodgeBack1)
    CKirbyDodgeBack1();
    virtual ~CKirbyDodgeBack1();

};
