#pragma once
#include "CBossState.h"

class CMorphoAtkG_Teleport_FireWall : public CBossState
{
private:
    CGameObject* m_FireWall;
    Ptr<CPrefab> m_FireWallPref;
    float m_WallSpeed;
    float m_AccTime;
    bool m_bFrmEnter;
    bool m_bSpawnDropStar;

public:
    virtual void tick() override;
    virtual void Exit() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Start();
    void Progress();
    void End();

    void MoveFireWall();

public:
    CLONE(CMorphoAtkG_Teleport_FireWall)
    CMorphoAtkG_Teleport_FireWall();
    virtual ~CMorphoAtkG_Teleport_FireWall();
};
