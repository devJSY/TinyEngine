#pragma once
#include "CState.h"
class CKirbyLadderUp : public CState
{
private:
    float m_SaveGravity;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyLadderUp)
    CKirbyLadderUp();
    virtual ~CKirbyLadderUp();

};
