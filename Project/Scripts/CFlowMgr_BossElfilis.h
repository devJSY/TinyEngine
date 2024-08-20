#pragma once
#include "CLevelFlowMgr.h"

class CFlowMgr_BossElfilis : public CLevelFlowMgr
{
private:
    BossLevelFlow m_FlowState;
    CGameObject* m_LevelEnterWall;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void TriggerEvent(UINT _Idx);

    void ChangeFlowFight();
    void ChangeFlowResist();
    void ChangeFlowDeath();

private:
    void SpawnElfilis();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CFlowMgr_BossElfilis);
    CFlowMgr_BossElfilis();
    CFlowMgr_BossElfilis(const CFlowMgr_BossElfilis& _Origin);
    virtual ~CFlowMgr_BossElfilis();
};
