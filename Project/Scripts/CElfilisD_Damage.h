#pragma once
#include "CBossState.h"

class CElfilisD_Damage : public CBossState
{
private:
    Vec3 m_TargetPos;
    Vec3 m_ForceDir;
    float m_PrevDrag;

public:
    virtual void tick();

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void Process();

public:
    CLONE(CElfilisD_Damage);
    CElfilisD_Damage();
    virtual ~CElfilisD_Damage();
};
