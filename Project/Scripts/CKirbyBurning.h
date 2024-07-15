#pragma once
#include "CState.h"
class CKirbyBurning : public CState
{
private:
    float m_SaveRotSpeed;
    float m_Duration;
    float m_Acc;
    float m_SaveSpeed;


public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyBurning)
    CKirbyBurning();
    virtual ~CKirbyBurning();
};
