#pragma once
#include "CBossState.h"

class CMorphoMoveG_Jump : public CBossState
{
private:
    float m_StartHeight;
    float m_JumpHeight;

public:
    virtual void tick() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void End();

public:
    CLONE(CMorphoMoveG_Jump)
    CMorphoMoveG_Jump();
    virtual ~CMorphoMoveG_Jump();
};