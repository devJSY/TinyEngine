#pragma once
#include "CState.h"


class CKirbyStageClear : public CState
{
private:
    UINT m_Step;
    
    float m_Acc;
    float m_Duration;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyStageClear)
    CKirbyStageClear();
    virtual ~CKirbyStageClear();

};
