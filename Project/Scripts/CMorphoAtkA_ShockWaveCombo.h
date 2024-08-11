#pragma once
#include "CBossState.h"

class CMorphoAtkA_ShockWaveCombo : public CBossState
{
public:
    virtual void tick() override;

private:
    void Enter_Step();
    void Exit_Step();

public:
    CLONE(CMorphoAtkA_ShockWaveCombo)
    CMorphoAtkA_ShockWaveCombo();
    virtual ~CMorphoAtkA_ShockWaveCombo();
};