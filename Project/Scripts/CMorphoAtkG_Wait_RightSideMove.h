#pragma once
#include "CBossState.h"

class CMorphoAtkG_Wait_RightSideMove : public CBossState
{
private:
    float m_AccTime;

public:
    virtual void tick() override;

private:
    void Enter_Step();
    void Exit_Step();

public:
    CLONE(CMorphoAtkG_Wait_RightSideMove)
    CMorphoAtkG_Wait_RightSideMove();
    virtual ~CMorphoAtkG_Wait_RightSideMove();
};
