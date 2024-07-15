#pragma once
#include "CState.h"
class CMonsterMgr;
class CMonsterMoveController;

class CNormalEnemyIdle 
	: public CState
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
    CLONE(CNormalEnemyIdle)
    CNormalEnemyIdle();
    virtual ~CNormalEnemyIdle();
};