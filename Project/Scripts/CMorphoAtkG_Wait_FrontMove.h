#pragma once
#include "CBossState.h"

class CMorphoAtkG_Wait_FrontMove : public CBossState
{
private:
    float m_AccTime;
    int m_SoundIdx;

public:
    virtual void tick() override;

private:
    void Enter_Step();
    void Exit_Step();

public:
    CLONE(CMorphoAtkG_Wait_FrontMove)
    CMorphoAtkG_Wait_FrontMove();
    virtual ~CMorphoAtkG_Wait_FrontMove();
};
