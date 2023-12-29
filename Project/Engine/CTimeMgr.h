#pragma once

class CTimeMgr : public CSingleton<CTimeMgr>
{
    SINGLE(CTimeMgr);

private:
    // Larget Integer 는 그냥 8바이트 long long 타입 정수로 취급
    LARGE_INTEGER m_Frequency;
    LARGE_INTEGER m_PrevCount;
    LARGE_INTEGER m_CurCount;
    float m_DeltaTime;

    UINT m_iCall;
    UINT m_iFPS;
    float m_fTime;

public:
    float GetDeltaTime() { return m_DeltaTime; }
    UINT GetFPS() const { return m_iFPS; }

public:
    void init();
    void tick();
};
