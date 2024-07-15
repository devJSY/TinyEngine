#pragma once
#include "CState.h"
class CNormalEnemyFind : public CState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CLONE(CNormalEnemyFind)
    CNormalEnemyFind();
    virtual ~CNormalEnemyFind();
};