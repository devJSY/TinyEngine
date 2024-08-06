#pragma once
#include "CState.h"
class CKirbyLadderWait : public CState
{
private:
    float m_SaveGravity;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyLadderWait)
    CKirbyLadderWait();
    virtual ~CKirbyLadderWait();

};
