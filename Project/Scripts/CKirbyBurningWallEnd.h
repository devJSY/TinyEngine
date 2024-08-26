#pragma once
#include "CState.h"

class CKirbyBurningWallEnd : public CState
{
private:
    float m_InitSpeed;
    float m_JumpPower;
    bool m_StateEnter;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyBurningWallEnd)
    CKirbyBurningWallEnd();
    virtual ~CKirbyBurningWallEnd();

};
