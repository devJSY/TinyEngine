#pragma once
#include "CState.h"

class CKirbyGuard : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyGuard)
    CKirbyGuard();
    virtual ~CKirbyGuard();
};
