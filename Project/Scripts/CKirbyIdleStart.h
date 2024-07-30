#pragma once
#include "CState.h"

class CKirbyIdleStart : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyIdleStart)
    CKirbyIdleStart();
    virtual ~CKirbyIdleStart();
};
