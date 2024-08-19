#pragma once
#include "CState.h"

class CKirbyVacuum2Start : public CState
{
private:
    float m_SavedSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyVacuum2Start)
    CKirbyVacuum2Start();
    virtual ~CKirbyVacuum2Start();
};
