#pragma once
#include "CState.h"

class CKirbyStuffedIdle : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyStuffedIdle)
    CKirbyStuffedIdle();
    virtual ~CKirbyStuffedIdle();
};
