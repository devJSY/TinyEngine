#pragma once
#include "CBossLevelFlowMgr.h"

class CFlowMgr_BossElfilis : public CBossLevelFlowMgr
{
private:
    CGameObject* m_LevelEnterWall;

public:
    virtual void begin() override;

    virtual void TriggerEvent(UINT _Idx);
    virtual void LevelEnd() override;
    virtual void LevelRestart() override;

private:
    // overrided flow events
    virtual void FlowLevelStart() override;
    virtual void EnterClear() override;
     
    // trigger events
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
