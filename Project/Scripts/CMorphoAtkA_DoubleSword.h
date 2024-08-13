#pragma once
#include "CBossState.h"

class CMorphoAtkA_DoubleSword : public CBossState
{
public:
    virtual void tick() override;

private:
    void Enter_Step();
    void Exit_Step();

public:
    CLONE(CMorphoAtkA_DoubleSword)
    CMorphoAtkA_DoubleSword();
    virtual ~CMorphoAtkA_DoubleSword();
};
