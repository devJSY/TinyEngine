#pragma once
#include "CState.h"
class CEnemyFind : public CState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CLONE(CEnemyFind)
    CEnemyFind();
    virtual ~CEnemyFind();
};