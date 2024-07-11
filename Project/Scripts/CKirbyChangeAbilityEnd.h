#pragma once
#include "CState.h"
class CKirbyChangeAbilityEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyChangeAbilityEnd)
    CKirbyChangeAbilityEnd();
    virtual ~CKirbyChangeAbilityEnd();
};
