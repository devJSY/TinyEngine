#pragma once
#include "CState.h"

class CKirbyRun : public CState
{
private:
    bool m_LastSmokeIsRight;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyRun)
    CKirbyRun();
    virtual ~CKirbyRun();
};
