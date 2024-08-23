#pragma once
#include "CBossState.h"

class CMorphoAtkG_NormalNear_Atk1 : public CBossState
{
private:
    Vec3 m_ForceDir;
    float m_PrevDrag;
    UINT m_ComboLevel;

public:
    virtual void tick() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Start();
    void Charge();
    void Progress();
    void End();

public:
    CLONE(CMorphoAtkG_NormalNear_Atk1)
    CMorphoAtkG_NormalNear_Atk1();
    virtual ~CMorphoAtkG_NormalNear_Atk1();
};
