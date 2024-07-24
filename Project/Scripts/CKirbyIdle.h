#pragma once
#include "CState.h"

enum class EmotionType
{
    NONE,
    WaveHand,
    Sit,
    Sit_Start,
    Sit_End,
    Yay,
};

class CKirbyIdle : public CState
{
private:
    vector<wstring> m_DefaultWaitAddAnim;
    EmotionType m_EmotionType;
    wstring m_PrevAnim;
    float m_WaitingTime;
    bool m_bPlayAddMotion;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

    void PlayWaitingAnim();

private:
    bool tick_Emotion();

public:
    CLONE(CKirbyIdle)
    CKirbyIdle();
    virtual ~CKirbyIdle();
};