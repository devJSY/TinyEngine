#pragma once
#include "CBossState.h"

class CMorphoAtkG_NormalNear_AtkFlight : public CBossState
{
private:
    float m_PrevDrag;

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
    CLONE(CMorphoAtkG_NormalNear_AtkFlight)
    CMorphoAtkG_NormalNear_AtkFlight();
    virtual ~CMorphoAtkG_NormalNear_AtkFlight();
};
