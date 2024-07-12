#pragma once
#include "CState.h"
class CKirbyChangeAbilityWait : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyChangeAbilityWait)
    CKirbyChangeAbilityWait();
    virtual ~CKirbyChangeAbilityWait();

};
