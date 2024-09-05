#pragma once

struct tSoundEvent
{
    Ptr<CSound> pSound;
    float StartVolume;
    float EndVolume;
    float Acc;               // 누적 시간
    float Duration;          // StartVolume ~ EndVolume 까지의 지속 시간
    bool bLoopAfterDuration; // 지속 시간이 끝난 후 Stop or Loop 여부
    bool bOverlap;

    // 3DSound Param
    bool b3DSound;
    Vec3 WorldPos;
    float Mindistance;
    float Maxdistance;
};

class CSoundMgr : public CSingleton<CSoundMgr>
{
    SINGLE(CSoundMgr);

private:
    std::list<tSoundEvent> m_ListSoundEvent;
    bool m_bSoundLock;

public:
    void tick();
    void FadeSound(const wstring& _SoundPath, float _StartVolume, float _EndVolume, float _Duration, bool _bLoopAfterDuration, bool _bOverlap = true,
                   bool _b3DSound = false, Vec3 _WorldPos = Vec3(), float _Mindistance = 100.f, float _Maxdistance = 1000.f);
    void ClearSoundEvent();

public:
    bool IsSoundLock() const { return m_bSoundLock; }
    void SetSoundLock(bool _bLock) { m_bSoundLock = _bLock; }
};
