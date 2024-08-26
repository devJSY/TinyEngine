#pragma once
#include "CBossState.h"

class CElfilisA_MoveR : public CBossState
{
private:
    Vec3 m_TargetPos;
    Vec3 m_StartPos;
    float m_PrevDrag;
    bool m_bMoveRight;
    bool m_bMoveUp;

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

    void Fly();

public:
    CLONE(CElfilisA_MoveR)
    CElfilisA_MoveR();
    virtual ~CElfilisA_MoveR();
};
