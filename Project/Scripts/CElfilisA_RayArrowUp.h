#pragma once
#include "CBossState.h"
#include "CElfilisFSM.h"
class CElfilisAirArrow;

class CElfilisA_RayArrowUp : public CBossState
{
private:
    Ptr<CPrefab> m_ArrowPref;
    CElfilisAirArrow* m_ArrowScript[7];
    ElfilisStateGroup m_NextStateGroup;
    float m_AccTime;
    bool m_bSpawn[7];
    bool m_bReady;
    bool m_bStateExit;

    // sound
    wstring m_SoundKeyArrowStart;
    wstring m_SoundKeyArrowSpawn;

public:
    virtual void tick() override;
    virtual void Exit() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Start();
    void Progress();
    void Wait();
    void End();

    void CastArrow();

public:
    CLONE(CElfilisA_RayArrowUp)
    CElfilisA_RayArrowUp();
    virtual ~CElfilisA_RayArrowUp();
};
