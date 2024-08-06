#pragma once
#include "CState.h"
class CKirbyLongDiveStart : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyLongDiveStart)
    CKirbyLongDiveStart();
    virtual ~CKirbyLongDiveStart();
};
