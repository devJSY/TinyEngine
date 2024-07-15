#pragma once
#include "CState.h"
class CKirbyBurningEnd : public CState
{
private:
    float m_SaveRotSpeed;
    float m_SaveSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyBurningEnd)
    CKirbyBurningEnd();
    virtual ~CKirbyBurningEnd();
};
