#pragma once
#include "CState.h"

class CKirbyVacuum1 : public CState
{
private:
    float m_SavedSpeed;
    float m_SaveRotSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyVacuum1)
    CKirbyVacuum1();
    virtual ~CKirbyVacuum1();
};
