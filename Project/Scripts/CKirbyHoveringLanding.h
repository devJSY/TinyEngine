#pragma once
#include "CState.h"

class CKirbyHoveringLanding : public CState
{
private:
    float m_SavedSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyHoveringLanding)
    CKirbyHoveringLanding();
    virtual ~CKirbyHoveringLanding();
};