#pragma once
#include "CState.h"
class CKirbyLongDiveLanding : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyLongDiveLanding)
    CKirbyLongDiveLanding();
    virtual ~CKirbyLongDiveLanding();
};
