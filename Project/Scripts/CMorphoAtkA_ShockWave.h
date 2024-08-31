#pragma once
#include "CBossState.h"
class CMorphoShockWave;

class CMorphoAtkA_ShockWave : public CBossState
{
private:
    vector<CMorphoShockWave*> m_vecShockWave;
    float m_AccTime;
    float m_ChargeTime;
    float m_WaitTime;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

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
    CLONE(CMorphoAtkA_ShockWave)
    CMorphoAtkA_ShockWave();
    virtual ~CMorphoAtkA_ShockWave();
};
