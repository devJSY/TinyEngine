#pragma once
#include "CBossState.h"

class CMorphoAtkA_DoubleSwordDivision : public CBossState
{
private:
    Ptr<CPrefab> m_ChargingParticlePref;
    CGameObject* m_ChargingParticle;
    float m_AccTime;
    float m_WaitTime;
    float m_ChargeTime;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Charge();
    void ChargeWait();
    void Progress();
    void End();

public:
    CLONE(CMorphoAtkA_DoubleSwordDivision)
    CMorphoAtkA_DoubleSwordDivision();
    virtual ~CMorphoAtkA_DoubleSwordDivision();
};
