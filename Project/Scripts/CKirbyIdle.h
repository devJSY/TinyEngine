#pragma once
#include "CState.h"

class CKirbyIdle : public CState
{
private:
    vector<wstring> m_WaitingAnim;
    wstring m_PrevAnim;
    float m_WaitingTime;
    bool m_bPlayAddMotion;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

    void PlayWaitingAnim();

public:
    CLONE(CKirbyIdle)
    CKirbyIdle();
    virtual ~CKirbyIdle();
};