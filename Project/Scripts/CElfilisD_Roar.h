#pragma once
#include "CState.h"

class CElfilisD_Roar : public CState
{
private:
    float m_AccTime;
    bool m_bFrmEnter;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CElfilisD_Roar);
    CElfilisD_Roar();
    virtual ~CElfilisD_Roar();
};