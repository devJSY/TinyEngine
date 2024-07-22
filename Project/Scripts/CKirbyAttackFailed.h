#pragma once
#include "CState.h"
class CKirbyAttackFailed : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackFailed)
    CKirbyAttackFailed();
    virtual ~CKirbyAttackFailed();

};
