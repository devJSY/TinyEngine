#pragma once
#include "CState.h"
class CNormalEnemyFailedAttack : public CState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CLONE(CNormalEnemyFailedAttack)
    CNormalEnemyFailedAttack();
    virtual ~CNormalEnemyFailedAttack();
};
