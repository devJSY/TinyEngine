#pragma once
#include "CState.h"

class CKirbyHoveringStart : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyHoveringStart)
    CKirbyHoveringStart();
    virtual ~CKirbyHoveringStart();
};
