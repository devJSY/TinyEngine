#pragma once
#include "CBossState.h"

class CElfilisD_Resist : public CBossState
{
private:
    float m_AccTime;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void ReadyStart();
    void Ready();
    void Start();
    void Progress();

public:
    CLONE(CElfilisD_Resist)
    CElfilisD_Resist();
    virtual ~CElfilisD_Resist();
};
