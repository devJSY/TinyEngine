#pragma once
#include "CState.h"

class CKirbyVacuum1Run : public CState
{
private:
    float m_SavedSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyVacuum1Run)
    CKirbyVacuum1Run();
    virtual ~CKirbyVacuum1Run();
};
