#pragma once
#include "CBossState.h"

class CElfilisG_GroundToAir : public CBossState
{
private:
    Vec3 m_StartPos;
    Vec3 m_TargetPos;
    float m_PrevDrag;

public:
    virtual void tick();

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void Progress();
    void End();

public:
    CLONE(CElfilisG_GroundToAir)
    CElfilisG_GroundToAir();
    virtual ~CElfilisG_GroundToAir();
};
