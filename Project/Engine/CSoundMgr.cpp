#include "pch.h"
#include "CSoundMgr.h"
#include "CTimeMgr.h"

CSoundMgr::CSoundMgr()
    : m_ListSoundEvent{}
{
}

CSoundMgr::~CSoundMgr()
{
}

void CSoundMgr::tick()
{
    std::list<tSoundEvent>::iterator iter = m_ListSoundEvent.begin();

    for (; iter != m_ListSoundEvent.end();)
    {
        tSoundEvent& Event = *iter;
        Event.Acc += DT_ENGINE;

        if (Event.Acc >= Event.Duration)
        {
            // 재생 시간이 지난 사운드 Stop
            if (!Event.bLoopAfterDuration)
            {
                Event.pSound->Stop();
            }

            iter = m_ListSoundEvent.erase(iter);
            continue;
        }

        float fVolume = Lerp(Event.StartVolume, Event.EndVolume, Event.Acc / Event.Duration);

        // 초기 사운드 재생
        if (!Event.pSound->IsPlaying())
        {
            if (Event.b3DSound)
            {
                Event.pSound->Play3D(Event.WorldPos, 0, fVolume, Event.bOverlap, Event.Mindistance, Event.Maxdistance);
            }
            else
            {
                Event.pSound->Play2D(0, fVolume, Event.bOverlap);
            }
        }
        // 재생중인 사운드는 Volume 만 설정
        else
        {
            Event.pSound->SetVolume(fVolume);
        }

        ++iter;
    }
}

void CSoundMgr::FadeSound(const wstring& _SoundPath, float _StartVolume, float _EndVolume, float _Duration, bool _bLoopAfterDuration, bool _bOverlap,
                          bool _b3DSound, Vec3 _WorldPos, float _Mindistance, float _Maxdistance)
{
    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

    // 존재하지 않는 사운드
    if (nullptr == pSound)
        return;

    std::list<tSoundEvent>::iterator iter = m_ListSoundEvent.begin();

    for (; iter != m_ListSoundEvent.end(); iter++)
    {
        // SoundMgr에 이미 등록된 사운드인 경우
        if (iter->pSound == pSound)
        {
            return;
        }
    }

    tSoundEvent SoundEvent;
    SoundEvent.pSound = pSound;
    SoundEvent.StartVolume = _StartVolume;
    SoundEvent.EndVolume = _EndVolume;
    SoundEvent.Acc = 0.f;
    SoundEvent.Duration = _Duration;
    SoundEvent.bLoopAfterDuration = _bLoopAfterDuration;
    SoundEvent.bOverlap = _bOverlap;
    SoundEvent.b3DSound = _b3DSound;
    SoundEvent.WorldPos = _WorldPos;
    SoundEvent.Mindistance = _Mindistance;
    SoundEvent.Maxdistance = _Maxdistance;

    m_ListSoundEvent.push_back(SoundEvent);
}
