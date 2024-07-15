#pragma once
#include "CState.h"

class CMonsterMgr;
class CMonsterMoveController;
class CNormalEnemyAttack : public CState
{
private:
    CTransform* m_pTransform;
    CMonsterMoveController* m_pMMC;
    CMonsterMgr* m_pMgr;

    float m_fAttackTime;
    float m_fAccTime;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CNormalEnemyAttack)
    CNormalEnemyAttack();
    virtual ~CNormalEnemyAttack();
};
