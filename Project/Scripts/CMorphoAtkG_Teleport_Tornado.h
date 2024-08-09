#pragma once
#include "CBossState.h"

class CMorphoAtkG_Teleport_Tornado : public CBossState
{
private:
    //CGameObject* m_FireWall;
    float m_TornadoSpeed;

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
    void Progress();
    void End();

public:
    CLONE(CMorphoAtkG_Teleport_Tornado)
    CMorphoAtkG_Teleport_Tornado();
    virtual ~CMorphoAtkG_Teleport_Tornado();
};
