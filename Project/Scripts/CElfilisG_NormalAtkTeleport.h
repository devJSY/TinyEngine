#pragma once
#include "CState.h"

class CElfilisG_NormalAtkTeleport : public CState
{
private:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CElfilisG_NormalAtkTeleport);
    CElfilisG_NormalAtkTeleport();
    virtual ~CElfilisG_NormalAtkTeleport();
};