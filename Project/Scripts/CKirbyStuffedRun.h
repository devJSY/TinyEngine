#pragma once
#include "CState.h"

class CKirbyStuffedRun : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyStuffedRun)
    CKirbyStuffedRun();
    virtual ~CKirbyStuffedRun();
};
