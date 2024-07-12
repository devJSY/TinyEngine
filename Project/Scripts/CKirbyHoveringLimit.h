#pragma once
#include "CState.h"
class CKirbyHoveringLimit : public CState
{
private:
    float m_SavedGravity;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyHoveringLimit)
    CKirbyHoveringLimit();
    virtual ~CKirbyHoveringLimit();
};