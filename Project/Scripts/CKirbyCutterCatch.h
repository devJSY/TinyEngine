#pragma once

#include "CState.h"

class CKirbyCutterCatch : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyCutterCatch)
    CKirbyCutterCatch();
    virtual ~CKirbyCutterCatch();
};
