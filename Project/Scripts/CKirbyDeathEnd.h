#pragma once
#include "CState.h"

class CKirbyDeathEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDeathEnd)
    CKirbyDeathEnd();
    virtual ~CKirbyDeathEnd();
};