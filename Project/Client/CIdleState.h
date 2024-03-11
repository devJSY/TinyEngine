#pragma once
#include <Engine\\CState.h>

class CIdleState : public CState
{

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CIdleState();
    virtual ~CIdleState();
};
