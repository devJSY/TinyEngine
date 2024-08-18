#pragma once
#include "CState.h"

class CKirbyVacuum1End : public CState
{
private:
    float m_SavedSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyVacuum1End)
    CKirbyVacuum1End();
    virtual ~CKirbyVacuum1End();
};
