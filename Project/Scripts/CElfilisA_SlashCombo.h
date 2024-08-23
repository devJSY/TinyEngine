#pragma once
#include "CBossState.h"

class CElfilisA_SlashCombo : public CBossState
{
private:
    Ptr<CPrefab> m_StabRockPref;
    Vec3 m_StartPos;
    Vec3 m_PrevPos;
    UINT m_ComboLevel;
    float m_SpawnDist;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Ready();
    void Start();
    void Progress();
    void Wait();

public:
    CLONE(CElfilisA_SlashCombo)
    CElfilisA_SlashCombo();
    virtual ~CElfilisA_SlashCombo();
};
