#pragma once
#include "CState.h"
class CKirbyDodgeBack2 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDodgeBack2)
    CKirbyDodgeBack2();
    virtual ~CKirbyDodgeBack2();
};
