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

public:
    float GetDeltaTime() const { return m_DeltaTime; }
    float GetEngineDeltaTime() const { return m_EngineDeltaTime; }
    UINT GetFPS() const { return m_iFPS; }
    void LockDeltaTime(bool _Lock) { m_bLock = _Lock; }

public:
    void init();
    void tick();
};
