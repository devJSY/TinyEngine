#pragma once
#include "CState.h"
class CKirbyAttackFailedEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackFailedEnd)
    CKirbyAttackFailedEnd();
    virtual ~CKirbyAttackFailedEnd();
};
