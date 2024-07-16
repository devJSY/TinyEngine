#pragma once
#include "CState.h"
class CEnemyFailedAttack : public CState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CLONE(CEnemyFailedAttack)
    CEnemyFailedAttack();
    virtual ~CEnemyFailedAttack();
};
