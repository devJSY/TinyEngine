#pragma once
#include "CLevelFlowMgr.h"

class CFlowMgr_BossMorpho : public CLevelFlowMgr
{
private:
    BossLevelFlow m_FlowState;
    CGameObject* m_SpawnButterfly;
    CGameObject* m_Barricade;
    Vec3 m_BarricadeScale;
    float m_AccTime;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void TriggerEvent(UINT _Idx);

    virtual void LevelStart() override;
    virtual void LevelEnd() override;
    virtual void LevelRestart() override;

    void ChangeFlowFight();
    void ChangeFlowDeath();
    void ChangeFlowClear();

public:
    BossLevelFlow GetFlowState() { return m_FlowState; }

private:
    void SpawnMorpho();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CFlowMgr_BossMorpho);
    CFlowMgr_BossMorpho();
    CFlowMgr_BossMorpho(const CFlowMgr_BossMorpho& _Origin);
    virtual ~CFlowMgr_BossMorpho();
};