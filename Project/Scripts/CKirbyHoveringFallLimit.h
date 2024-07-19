#pragma once
#include "CState.h"

class CKirbyHoveringFallLimit : public CState
{
private:
    float m_SavedGravity;
    float m_SavedSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyHoveringFallLimit)
    CKirbyHoveringFallLimit();
    virtual ~CKirbyHoveringFallLimit();
};