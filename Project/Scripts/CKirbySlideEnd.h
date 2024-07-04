#pragma once
#include "CState.h"
class CKirbySlideEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbySlideEnd)
    CKirbySlideEnd();
    virtual ~CKirbySlideEnd();
};
