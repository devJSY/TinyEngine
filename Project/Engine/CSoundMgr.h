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

public:
    void tick();
    void FadeSound(const wstring& _SoundPath, float _StartVolume, float _EndVolume, float _Duration, bool _bLoopAfterDuration, bool _bOverlap = true,
                   bool _b3DSound = false, Vec3 _WorldPos = Vec3(), float _Mindistance = 1.f, float _Maxdistance = 500.f);
    void ClearSoundEvent();
};
