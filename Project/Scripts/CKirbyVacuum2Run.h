#pragma once
#include "CState.h"

class CKirbyVacuum2Run : public CState
{
private:
    float m_SavedSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyVacuum2Run)
    CKirbyVacuum2Run();
    virtual ~CKirbyVacuum2Run();
};
