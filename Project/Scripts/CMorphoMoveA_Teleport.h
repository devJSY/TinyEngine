#pragma once
#include "CBossState.h"

class CMorphoMoveA_Teleport : public CBossState
{
private:
    Vec3 m_AfterPos;
    float m_AccTime;
    float m_WaitTime;

public:
    virtual void tick() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void End();

public:
    CLONE(CMorphoMoveA_Teleport)
    CMorphoMoveA_Teleport();
    virtual ~CMorphoMoveA_Teleport();
};