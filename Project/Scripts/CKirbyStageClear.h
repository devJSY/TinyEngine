#pragma once
#include "CState.h"

enum class StateStep
{

};


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


private:

public:
    CLONE(CKirbyStageClear)
    CKirbyStageClear();
    virtual ~CKirbyStageClear();

};
