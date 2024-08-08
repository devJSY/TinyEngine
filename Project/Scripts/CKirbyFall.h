#pragma once
#include "CState.h"
class CKirbyFall : public CState
{
private:
    float m_Acc;
    float m_TimeStopDuration;
    float m_Duration;

    bool m_TimeStopExit;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyFall)
    CKirbyFall();
    virtual ~CKirbyFall();

};
