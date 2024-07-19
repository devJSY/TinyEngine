#pragma once
#include "CState.h"

class CKirbyDeath : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDeath)
    CKirbyDeath();
    virtual ~CKirbyDeath();
};