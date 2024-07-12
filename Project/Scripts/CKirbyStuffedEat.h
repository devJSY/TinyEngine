#pragma once
#include "CState.h"

class CKirbyStuffedEat : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyStuffedEat)
    CKirbyStuffedEat();
    virtual ~CKirbyStuffedEat();
};
