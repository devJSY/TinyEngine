#pragma once
#include "CEntity.h"

class CFSM;

class CState : public CEntity
{
private:
    CFSM* m_FSM;

public:
    virtual void finaltick() = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;

public:
    CLONE_DISABLE(CState);

public:
    CState();
    virtual ~CState();

    friend class CFSM;
};
