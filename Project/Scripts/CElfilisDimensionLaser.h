#pragma once
#include <Engine\\CScript.h>
class CElfilisLaser;

class CElfilisDimensionLaser : public CScript
{
private:
    vector<CGameObject*> m_vecDimension;
    CElfilisLaser* m_Laser;
    UINT m_Step;
    float m_AccTime;
    float m_PlayTime;
    bool m_bLaserStartLock;
    bool m_bLaserRepeat;

public:
    virtual void begin() override;
    virtual void tick() override;

    void PlaySpawn();
    void PlayStartLaser(float _PlayTime = 3.f);
    void PlayEndLaser();
    void Reset();

public:
    void SetLaserStartLcok(bool _bLock) { m_bLaserStartLock = _bLock; }
    void SetLaserRepeat(bool _bRepeat) { m_bLaserRepeat = _bRepeat; }

    bool IsSpawnFinish() { return m_Step > 1; }
    bool IsLaserStart() { return m_Step >= 3; }
    bool IsLaserEnd() { return m_Step >= 5; }
    bool IsDisappearFinish() { return m_Step > 6; }

private:
    // step
    void Spawn();
    void Wait();
    void StartAttack();
    void Progress();
    void EndAttack();
    void Disappear();

    void ParseChild();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisDimensionLaser);
    CElfilisDimensionLaser();
    virtual ~CElfilisDimensionLaser();
};
