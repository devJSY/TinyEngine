#pragma once
#include "CLevelFlowMgr.h"

class CFlowMgr_BossMorpho : public CLevelFlowMgr
{
private:
    BossLevelFlow m_FlowState;
    CGameObject* m_SpawnButterfly;
    CGameObject* m_Barricade;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void TriggerEvent(UINT _Idx);

    virtual void LevelStart() override;

    void SetFight();

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