#pragma once
#include "CState.h"

class CKirbyVacuum2 : public CState
{
private:
    float m_SavedSpeed;
    float m_SaveRotSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyVacuum2)
    CKirbyVacuum2();
    virtual ~CKirbyVacuum2();
};
