#pragma once
#include "CState.h"

class CKirbyHoveringSpit : public CState
{
private:
    Ptr<CPrefab> m_SpitSmoke;

public:
    virtual void tick();    
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyHoveringSpit)
    CKirbyHoveringSpit();
    CKirbyHoveringSpit(const CKirbyHoveringSpit& _Origin);
    virtual ~CKirbyHoveringSpit();
};