#pragma once
#include "CBossState.h"

class CElfilisD_Appear : public CBossState
{
private:
    Ptr<CPrefab> m_BossNamePref;
    CGameObject* m_BossName;
    Vec3 m_StartPos;
    float m_DownSpeed;
    float m_AccTime;

public:
    virtual void tick();

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void Progress();

public:
    CLONE(CElfilisD_Appear);
    CElfilisD_Appear();
    virtual ~CElfilisD_Appear();
};
