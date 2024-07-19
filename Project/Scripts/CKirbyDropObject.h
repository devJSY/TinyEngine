#pragma once
#include "CState.h"

class CKirbyDropObject : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDropObject)
    CKirbyDropObject();
    virtual ~CKirbyDropObject();
};