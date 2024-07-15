#pragma once

#include "CState.h"

class CMonsterMgr;
class CMonsterMoveController;
class CNormalEnemyPatrol
	:public CState
{
private:
    CTransform* m_pTransform;
    CMonsterMoveController* m_pMMC;
    CMonsterMgr* m_pMgr;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CNormalEnemyPatrol)
    CNormalEnemyPatrol();
    virtual ~CNormalEnemyPatrol();
};
