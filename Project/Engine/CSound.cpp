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

int CSound::Play2D(int _iRoopCount, float _fVolume, bool _bOverlap)
{
    if (_iRoopCount <= -1)
    {
        assert(nullptr);
        return E_FAIL;
    }

    // 재생되고 있는 채널이 있는데, 중복재생을 허용하지 않았다 -> 재생 안함
    if (!_bOverlap && !m_listChannel.empty())
    {
        return E_FAIL;
    }

    _iRoopCount -= 1;

    FMOD::Channel* pChannel = nullptr;
    g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);

    // 재생 실패
    if (nullptr == pChannel)
        return E_FAIL;

    _fVolume = std::clamp(_fVolume, 0.f, 1.f);
    pChannel->setVolume(_fVolume);

    pChannel->setCallback(CHANNEL_CALLBACK);
    pChannel->setUserData(this);

    pChannel->setMode(FMOD_LOOP_NORMAL | FMOD_2D);
    pChannel->setLoopCount(_iRoopCount);

    m_listChannel.push_back(pChannel);

    int iIdx = -1;
    pChannel->getIndex(&iIdx);
    m_bPaused = false;

    return iIdx;
}

int CSound::Play3D(Vec3 _WorldPos, int _iRoopCount, float _fVolume, bool _bOverlap, float _Mindistance, float _Maxdistance)
{
    if (_iRoopCount <= -1)
    {
        assert(nullptr);
        return E_FAIL;
    }

    // 재생되고 있는 채널이 있는데, 중복재생을 허용하지 않았다 -> 재생 안함
    if (!_bOverlap && !m_listChannel.empty())
    {
        return E_FAIL;
    }

    // Distance 가 음수인 경우
    if (_Mindistance < 0.f || _Maxdistance < 0.f)
    {
        return E_FAIL;
    }

    _iRoopCount -= 1;

    FMOD::Channel* pChannel = nullptr;
    g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);

    // 재생 실패
    if (nullptr == pChannel)
        return E_FAIL;

    // Distance 예외처리
    if (_Mindistance > _Maxdistance)
    {
        std::swap(_Mindistance, _Maxdistance);
    }

    FMOD_VECTOR pos = {_WorldPos.x, _WorldPos.y, _WorldPos.z};
    pChannel->set3DAttributes(&pos, nullptr);
    pChannel->set3DMinMaxDistance(_Mindistance, _Maxdistance);

    _fVolume = std::clamp(_fVolume, 0.f, 1.f);
    pChannel->setVolume(_fVolume);

    pChannel->setCallback(CHANNEL_CALLBACK);
    pChannel->setUserData(this);

    pChannel->setMode(FMOD_LOOP_NORMAL | FMOD_3D);
    pChannel->setLoopCount(_iRoopCount);

    m_listChannel.push_back(pChannel);

    int iIdx = -1;
    pChannel->getIndex(&iIdx);
    m_bPaused = false;

    return iIdx;
}

void CSound::Stop()
{
    m_bPaused = false;

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

int CSound::GetLength()
{
    unsigned int length;
    m_pSound->getLength(&length, FMOD_TIMEUNIT_MS);
    return length;
}

int CSound::GetPosition()
{
    list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

    unsigned int position = 0;
    for (; iter != m_listChannel.end(); ++iter)
    {
        (*iter)->getPosition(&position, FMOD_TIMEUNIT_MS);
        return position;
    }

    return position;
}

void CSound::SetPosition(int _Position)
{
    list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

    for (; iter != m_listChannel.end(); iter++)
    {
        (*iter)->setPosition(_Position, FMOD_TIMEUNIT_MS); // 채널의 재생 위치를 설정
    }
}

float CSound::GetPitch()
{
    list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

    float Pitch = 0;
    for (; iter != m_listChannel.end(); ++iter)
    {
        (*iter)->getPitch(&Pitch);
        return Pitch;
    }

    return Pitch;
}

void CSound::SetPitch(float _Pitch)
{
    list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

    for (; iter != m_listChannel.end(); iter++)
    {
        (*iter)->setPitch(_Pitch);
    }
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

void CSound::SetVolume(float _Volume)
{
    list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

    for (; iter != m_listChannel.end(); ++iter)
    {
        (*iter)->setVolume(_Volume);
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
    if (FMOD_OK != g_pFMOD->createSound(ToString(_strFilePath).c_str(), FMOD_DEFAULT | FMOD_3D, nullptr, &m_pSound))
    {
        return E_FAIL;
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