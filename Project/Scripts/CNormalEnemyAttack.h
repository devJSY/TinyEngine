#pragma once
#include "CState.h"
class CNormalEnemyAttack : public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CNormalEnemyAttack)
    CNormalEnemyAttack();
    virtual ~CNormalEnemyAttack();
};
