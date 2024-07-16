#pragma once
#include "CState.h"
class CEnemySuccessedAttack 
	: public CState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CLONE(CEnemySuccessedAttack)
    CEnemySuccessedAttack();
    virtual ~CEnemySuccessedAttack();
};
