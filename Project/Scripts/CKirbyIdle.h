#pragma once
#include "CState.h"

class CKirbyIdle : public CState
{
private:
    float m_WaitingAccTime;
    bool m_bPlayWaiting;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyIdle)
    CKirbyIdle();
    virtual ~CKirbyIdle();
};