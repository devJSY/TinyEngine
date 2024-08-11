#pragma once
#include "CBossState.h"

class CMorphoMoveG_TeleportFar : public CBossState
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
    CLONE(CMorphoMoveG_TeleportFar)
    CMorphoMoveG_TeleportFar();
    virtual ~CMorphoMoveG_TeleportFar();
};