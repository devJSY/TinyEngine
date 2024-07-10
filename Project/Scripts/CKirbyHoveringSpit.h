#pragma once
#include "CState.h"

class CKirbyHoveringSpit : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyHoveringSpit)
    CKirbyHoveringSpit();
    virtual ~CKirbyHoveringSpit();
};