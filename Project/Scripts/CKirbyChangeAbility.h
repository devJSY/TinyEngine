#pragma once
#include "CState.h"

class CKirbyChangeAbility : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyChangeAbility)
    CKirbyChangeAbility();
    virtual ~CKirbyChangeAbility();
};
