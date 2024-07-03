#pragma once
#include "CState.h"
class CKirbyDodgeFront2 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDodgeFront2)
    CKirbyDodgeFront2();
    virtual ~CKirbyDodgeFront2();
};
