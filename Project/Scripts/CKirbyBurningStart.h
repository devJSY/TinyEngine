#pragma once
#include "CState.h"
class CKirbyBurningStart : public CState
{
private:
    float m_SaveRotSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyBurningStart)
    CKirbyBurningStart();
    virtual ~CKirbyBurningStart();
};
