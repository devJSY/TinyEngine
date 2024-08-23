#pragma once
#include "CBossState.h"

class CMorphoAtkG_NormalNear_Atk2 : public CBossState
{
private:
    Vec3 m_ForceDir;
    float m_PrevDrag;

public:
    virtual void tick() override;
    virtual void Enter() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Progress();
    void End();

public:
    CLONE(CMorphoAtkG_NormalNear_Atk2)
    CMorphoAtkG_NormalNear_Atk2();
    virtual ~CMorphoAtkG_NormalNear_Atk2();
};
