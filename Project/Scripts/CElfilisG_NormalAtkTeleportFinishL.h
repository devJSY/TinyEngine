#pragma once
#include "CBossState.h"

class CElfilisG_NormalAtkTeleportFinishL : public CBossState
{
public:
    virtual void tick() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Start();
    void Progress();
    void End();

public:
    CLONE(CElfilisG_NormalAtkTeleportFinishL)
    CElfilisG_NormalAtkTeleportFinishL();
    virtual ~CElfilisG_NormalAtkTeleportFinishL();
};
