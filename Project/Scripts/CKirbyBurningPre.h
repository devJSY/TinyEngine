#pragma once
#include "CState.h"
class CKirbyBurningPre : public CState
{
private:
    float m_SaveGravity;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyBurningPre)
    CKirbyBurningPre();
    virtual ~CKirbyBurningPre();
};
