#pragma once

class CTimeMgr : public CSingleton<CTimeMgr>
{
    SINGLE(CTimeMgr);

private:
    LARGE_INTEGER m_Frequency;
    LARGE_INTEGER m_PrevCount;
    LARGE_INTEGER m_CurCount;

    float m_DeltaTime;
    float m_EngineDeltaTime;

    UINT m_iCall;
    UINT m_iFPS;
    float m_fAccTime;

    bool m_bLock;

    float m_TimeScale;
    float m_TimeScaleDuration;
    bool m_TimeScaling;

public:
    float GetDeltaTime() const { return m_DeltaTime; }
    float GetEngineDeltaTime() const { return m_EngineDeltaTime; }
    UINT GetFPS() const { return m_iFPS; }
    void LockDeltaTime(bool _Lock) { m_bLock = _Lock; }

    float GetTimeScale() const { return m_TimeScale; }
    void SetTimeScale(float _TimeScale) { m_TimeScale = _TimeScale; }
    void SetTimeScale(float _Duration, float _TimeScale);

public:
    void init();
    void tick();
};
