#pragma once
#include "CBossState.h"

class CElfilisD_Jump : public CBossState
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
    CLONE(CElfilisD_Jump);
    CElfilisD_Jump();
    virtual ~CElfilisD_Jump();
};
