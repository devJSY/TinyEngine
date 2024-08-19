#pragma once
#include "CBossState.h"

class CMorphoDemo_Appear : public CBossState
{
private:
    Ptr<CPrefab> m_BossNamePref;
    CGameObject* m_BossName;
    Vec3 m_StartPos;
    float m_DownSpeed;
    float m_AccTime;
    bool m_bFrmEnter;
    bool m_bFrmEnter2;

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

public:
    CLONE(CMorphoDemo_Appear)
    CMorphoDemo_Appear();
    virtual ~CMorphoDemo_Appear();
};
