#pragma once
#include "CState.h"

class CKirbyAttackEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackEnd)
    CKirbyAttackEnd();
    virtual ~CKirbyAttackEnd();
};
