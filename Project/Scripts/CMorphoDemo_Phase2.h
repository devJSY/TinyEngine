#pragma once
#include "CBossState.h"

class CMorphoDemo_Phase2 : public CBossState
{
private:
    float m_AccTime;

public:
    virtual void tick() override;
    virtual void Exit() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void StartEnd();
    void Progress();

public:
    CLONE(CMorphoDemo_Phase2)
    CMorphoDemo_Phase2();
    virtual ~CMorphoDemo_Phase2();
};