#pragma once
#include "CState.h"
class CKirbySlideStart : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbySlideStart)
    CKirbySlideStart();
    virtual ~CKirbySlideStart();
};
