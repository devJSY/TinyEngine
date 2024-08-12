#pragma once
#include "CBossState.h"

class CMorphoAtkG_Teleport_Tornado : public CBossState
{
private:
    float m_AccTime;
    float m_ChargeTime;
    float m_TornadoTime;

    // tornado
    Ptr<CPrefab> m_ChargeEffect;
    Ptr<CPrefab> m_TornadoPref;
    CGameObject* m_Tornado[6];
    float m_TornadoSpeed;

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
    void Progress();
    void Wait();
    void End();

public:
    CLONE(CMorphoAtkG_Teleport_Tornado)
    CMorphoAtkG_Teleport_Tornado();
    virtual ~CMorphoAtkG_Teleport_Tornado();
};
