#include "pch.h"
#include "CSound.h"

// CallBack 함수 전방선언
FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype,
                             FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2);

FMOD::System* CSound::g_pFMOD = nullptr;

CSound::CSound()
    : CAsset(ASSET_TYPE::SOUND, false)
    , m_pSound(nullptr)
    , m_bPaused(false)
{
}

CSound::~CSound()
{
    if (nullptr != m_pSound)
    {
        m_pSound->release();
        m_pSound = nullptr;
    }
}

int CSound::Play(int _iRoopCount, float _fVolume, bool _bOverlap)
{
    if (_iRoopCount <= -1)
    {
        assert(nullptr);
    }

    if (!m_listChannel.empty())
    {
        // 일시 정지 상태인경우 해제
        if (m_bPaused)
        {
            Pause(false);
            return S_OK;
        }
        // 재생되고 있는 채널이 있는데, 중복재생을 허용하지 않았다 -> 재생 안함
        else if (!_bOverlap && !m_listChannel.empty())
        {
            return E_FAIL;
        }
    }

    _iRoopCount -= 1;

    FMOD::Channel* pChannel = nullptr;
    g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);

    // 재생 실패
    if (nullptr == pChannel)
        return E_FAIL;

    pChannel->setVolume(_fVolume);

    pChannel->setCallback(CHANNEL_CALLBACK);
    pChannel->setUserData(this);

    pChannel->setMode(FMOD_LOOP_NORMAL);
    pChannel->setLoopCount(_iRoopCount);

    m_listChannel.push_back(pChannel);

    int iIdx = -1;
    pChannel->getIndex(&iIdx);
    m_bPaused = false;

    return iIdx;
}

void CSound::Stop()
{
    list<FMOD::Channel*>::iterator iter;

    while (!m_listChannel.empty())
    {
        iter = m_listChannel.begin();
        (*iter)->stop();
    }
}

void CSound::Pause(bool _Pause)
{
    list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

    for (; iter != m_listChannel.end(); iter++)
    {
        (*iter)->setPaused(_Pause);
    }

    m_bPaused = _Pause;
}

bool CSound::IsPlaying()
{
    if (m_bPaused)
        return false;

    bool playing = false;
    list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

    for (; iter != m_listChannel.end(); iter++)
    {
        (*iter)->isPlaying(&playing);

        if (playing)
            return playing;
    }

    return playing;
}

void CSound::SetVolume(float _Volume, int _iChannelIdx)
{
    list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

    int iIdx = -1;
    for (; iter != m_listChannel.end(); ++iter)
    {
        (*iter)->getIndex(&iIdx);
        if (_iChannelIdx == iIdx)
        {
            (*iter)->setVolume(_Volume);
            return;
        }
    }
}

void CSound::RemoveChannel(FMOD::Channel* _pTargetChannel)
{
    list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

    for (; iter != m_listChannel.end(); ++iter)
    {
        if (*iter == _pTargetChannel)
        {
            m_listChannel.erase(iter);
            return;
        }
    }
}

int CSound::Load(const wstring& _strFilePath)
{
    if (FMOD_OK != g_pFMOD->createSound(ToString(_strFilePath).c_str(), FMOD_DEFAULT, nullptr, &m_pSound))
    {
        assert(nullptr);
    }

    return S_OK;
}

// =========
// Call Back
// =========
FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype,
                             FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2)
{
    FMOD::Channel* cppchannel = (FMOD::Channel*)channelcontrol;
    CSound* pSound = nullptr;

    switch (controltype)
    {
    case FMOD_CHANNELCONTROL_CALLBACK_END: {
        cppchannel->getUserData((void**)&pSound);
        pSound->RemoveChannel(cppchannel);
    }
    break;
    }

    return FMOD_OK;
}