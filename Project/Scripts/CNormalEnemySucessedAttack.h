#pragma once
#include "CState.h"
class CNormalEnemySucessedAttack 
	: public CState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CLONE(CNormalEnemySucessedAttack)
    CNormalEnemySucessedAttack();
    virtual ~CNormalEnemySucessedAttack();
};
