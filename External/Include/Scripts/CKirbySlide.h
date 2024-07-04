#pragma once
#include "CState.h"
class CKirbySlide : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbySlide)
    CKirbySlide();
    virtual ~CKirbySlide();

};
