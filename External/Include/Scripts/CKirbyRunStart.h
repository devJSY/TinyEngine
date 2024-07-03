#pragma once
#include "CState.h"

class CKirbyRunStart : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyRunStart)
    CKirbyRunStart();
    virtual ~CKirbyRunStart();
};
