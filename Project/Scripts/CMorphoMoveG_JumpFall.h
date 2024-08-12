#pragma once
#include "CBossState.h"

class CMorphoMoveG_JumpFall : public CBossState
{
public:
    virtual void tick() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void End();

public:
    CLONE(CMorphoMoveG_JumpFall)
    CMorphoMoveG_JumpFall();
    virtual ~CMorphoMoveG_JumpFall();
};