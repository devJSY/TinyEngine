#pragma once
#include "CState.h"
class CKirbySlide : public CState
{
private:
    CGameObject* m_Spawner;

    float   m_Speed;
    float   m_Acc;
    float   m_Duration;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbySlide)
    CKirbySlide();
    virtual ~CKirbySlide();

};
