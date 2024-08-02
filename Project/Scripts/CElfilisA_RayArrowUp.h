#pragma once
#include "CBossState.h"
class CElfilisAirArrow;

class CElfilisA_RayArrowUp : public CBossState
{
private:
    CGameObject* m_Arrow[7];
    CElfilisAirArrow* m_ArrowScript[7];
    float m_AccTime;
    bool m_bSpawn[7];
    bool m_bReady;

public:
    virtual void tick() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Start();
    void Progress();
    void Wait();
    void End();

public:
    CLONE(CElfilisA_RayArrowUp)
    CElfilisA_RayArrowUp();
    virtual ~CElfilisA_RayArrowUp();
};
