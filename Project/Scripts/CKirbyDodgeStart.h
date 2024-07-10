#pragma once
#include "CState.h"
class CKirbyDodgeStart : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDodgeStart)
    CKirbyDodgeStart();
    virtual ~CKirbyDodgeStart();
};
