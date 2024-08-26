#pragma once
#include "CBossState.h"

class CMorphoDemo_Death : public CBossState
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
    void Ready();
    void Start();
    void Wait();
    void End();

public:
    CLONE(CMorphoDemo_Death)
    CMorphoDemo_Death();
    virtual ~CMorphoDemo_Death();
};
