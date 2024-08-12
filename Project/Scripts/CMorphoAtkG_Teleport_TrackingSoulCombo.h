#pragma once
#include "CBossState.h"

class CMorphoAtkG_Teleport_TrackingSoulCombo : public CBossState
{
public:
    virtual void tick() override;

private:
    void Enter_Step();
    void Exit_Step();

public:
    CLONE(CMorphoAtkG_Teleport_TrackingSoulCombo)
    CMorphoAtkG_Teleport_TrackingSoulCombo();
    virtual ~CMorphoAtkG_Teleport_TrackingSoulCombo();
};
