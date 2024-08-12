#pragma once
#include "CBossState.h"

class CMorphoMoveG_TeleportCombo : public CBossState
{
public:
    virtual void tick() override;

private:
    void Enter_Step();
    void Exit_Step();

public:
    CLONE(CMorphoMoveG_TeleportCombo)
    CMorphoMoveG_TeleportCombo();
    virtual ~CMorphoMoveG_TeleportCombo();
};