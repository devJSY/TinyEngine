#pragma once
#pragma once
#include "CAsset.h"

#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
    #pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
    #pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif

class CSound : public CAsset
{
public:
    static FMOD::System* g_pFMOD; // FMOD ������ Ŭ����

public:
    FMOD::Sound* m_pSound;              // �ε��� ���� ��ü
    list<FMOD::Channel*> m_listChannel; // ���尡 ����ǰ� �ִ� ä��
    bool m_bPaused;

public:
    // _iRoopCount : 0 (���ѹݺ�),  _fVolume : 0 ~ 1(Volume), _bOverlap : �Ҹ� ��ø ���ɿ���
    int Play2D(int _iRoopCount, float _fVolume = 0.5f, bool _bOverlap = false);
    int Play3D(Vec3 _WorldPos, int _iRoopCount, float _fVolume = 0.5f, bool _bOverlap = false, float _Mindistance = 100.f, float _Maxdistance = 1000.f);
    void Stop();
    void Pause(bool _Pause = true);

    bool IsPlaying();
    bool IsPaused() const { return m_bPaused; }

    int GetLength();
    int GetPosition();
    void SetPosition(int _Position);

    float GetPitch();
    void SetPitch(float _Pitch = 1.f);

    // 0 ~ 1
    void SetVolume(float _Volume, int _iChannelIdx);
    void SetVolume(float _Volume);

private:
    void RemoveChannel(FMOD::Channel* _pTargetChannel);
    friend FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype,
                                        FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2);

public:
    virtual int Save(const wstring&) override { return S_OK; }
    virtual int Load(const wstring& _strFilePath) override;

    CLONE_DISABLE(CSound);

public:
    CSound();
    virtual ~CSound();
};
