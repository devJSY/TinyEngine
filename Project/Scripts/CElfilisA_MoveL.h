#pragma once
#include "CBossState.h"

class CElfilisA_MoveL : public CBossState
{
private:
    Vec3 m_TargetPos;
    Vec3 m_StartPos;
    float m_PrevDrag;
    bool m_bFrmEnter;
    bool m_bMoveUp;

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
    CLONE(CElfilisA_MoveL)
    CElfilisA_MoveL();
    virtual ~CElfilisA_MoveL();
};
