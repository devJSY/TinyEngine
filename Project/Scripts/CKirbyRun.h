#pragma once
#include "CState.h"

class CKirbyRun : public CState
{
private:
    bool m_FirstStep;

    int m_FirstStepSmokeFrm;
    int m_SecondStepSmokeFrm;
    float m_SmokeOffset;


public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();


private:
    void SettingSmoke();
    void SpawnSmoke();

public:
    CLONE(CKirbyRun)
    CKirbyRun();
    virtual ~CKirbyRun();
};
