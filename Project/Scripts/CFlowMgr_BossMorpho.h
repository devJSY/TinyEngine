#pragma once
#include "CBossLevelFlowMgr.h"

class CFlowMgr_BossMorpho : public CBossLevelFlowMgr
{
private:
    CGameObject* m_SpawnButterfly;
    CGameObject* m_Barricade;
    Vec3 m_BarricadeScale;
    float m_AccTime;

public:
    virtual void begin() override;
    virtual void TriggerEvent(UINT _Idx);

    virtual void LevelEnd() override;
    virtual void LevelRestart() override;

private:
    // overrided flow events
    virtual void FlowLevelStart() override;
    virtual void EnterWaitBoss() override;
    virtual void FlowWaitBoss() override;
    virtual void EnterFight() override;
    virtual void EnterClear() override;

    // trigger events
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