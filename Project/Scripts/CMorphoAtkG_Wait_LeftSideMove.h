#pragma once
#include "CBossState.h"

class CMorphoAtkG_Wait_LeftSideMove : public CBossState
{
private:
    float m_AccTime;

public:
    virtual void tick() override;

private:
    void Enter_Step();
    void Exit_Step();

public:
    CLONE(CMorphoAtkG_Wait_LeftSideMove)
    CMorphoAtkG_Wait_LeftSideMove();
    virtual ~CMorphoAtkG_Wait_LeftSideMove();
};