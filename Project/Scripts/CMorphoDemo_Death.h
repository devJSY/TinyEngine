#pragma once
#include "CBossState.h"

class CMorphoDemo_Death : public CBossState
{
private:
    Ptr<CPrefab> m_ParticleMorphoDeath;
    Ptr<CPrefab> m_ParticleMorphoDust;
    float m_AccTime;
    bool m_bFrmEnter;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Ready();
    void Start();
    void Wait();
    void End();

public:
    CLONE(CMorphoDemo_Death)
    CMorphoDemo_Death();
    virtual ~CMorphoDemo_Death();
};
