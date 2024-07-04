#pragma once
#include "CState.h"
class CKirbyDodgeFront1 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDodgeFront1)
    CKirbyDodgeFront1();
    virtual ~CKirbyDodgeFront1();
};
