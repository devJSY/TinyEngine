#pragma once
#include "CState.h"

class CKirbyHoveringStart : public CState
{
private:
    float m_SavedGravity;
    float m_SavedSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyHoveringStart)
    CKirbyHoveringStart();
    virtual ~CKirbyHoveringStart();
};
