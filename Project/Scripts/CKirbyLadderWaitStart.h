#pragma once

#include "CState.h"

class CKirbyLadderWaitStart : public CState
{
private:
    float m_SaveGravity;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyLadderWaitStart)
    CKirbyLadderWaitStart();
    virtual ~CKirbyLadderWaitStart();

};
