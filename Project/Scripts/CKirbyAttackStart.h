#pragma once
#include "CState.h"

class CKirbyAttackStart : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackStart)
    CKirbyAttackStart();
    virtual ~CKirbyAttackStart();
};
