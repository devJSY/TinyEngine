#pragma once
#include "CState.h"

class CKirbyChangeObjectEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyChangeObjectEnd)
    CKirbyChangeObjectEnd();
    virtual ~CKirbyChangeObjectEnd();
};
