#pragma once
#include "CState.h"

class CKirbyChangeObject : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyChangeObject)
    CKirbyChangeObject();
    virtual ~CKirbyChangeObject();
};