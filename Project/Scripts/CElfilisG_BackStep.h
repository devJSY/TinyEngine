#pragma once
#include "CBossState.h"

class CElfilisG_BackStep : public CBossState
{
private:
    Vec3 m_StartPos;
    Vec3 m_TargetPos;
    Vec3 m_ForceDir;
    float m_PrevDrag;
    bool m_bFinishMove;
    bool m_bWall;

public:
    virtual void tick() override;
    virtual void Exit() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void Progress();
    void End();

    void MoveBack();

public:
    CLONE(CElfilisG_BackStep)
    CElfilisG_BackStep();
    virtual ~CElfilisG_BackStep();
};
