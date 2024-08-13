#pragma once
#include "CBossState.h"

class CMorphoAtkG_Teleport_TrackingSoul : public CBossState
{
private:
    float m_AccTime;
    float m_ChargeTime;
    float m_WaitTime;

    // tornado
    Ptr<CPrefab> m_SoulPref;

public:
    virtual void tick() override;
    virtual void Enter() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Charge();
    void ChargeWait();
    void Start();
    void Wait();
    void End();

public:
    CLONE(CMorphoAtkG_Teleport_TrackingSoul)
    CMorphoAtkG_Teleport_TrackingSoul();
    virtual ~CMorphoAtkG_Teleport_TrackingSoul();
};
