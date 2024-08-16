#pragma once

#include "CLevelFlowMgr.h"

class CFlowMgr_LvStart : public CLevelFlowMgr
{
private:
    CGameObject* m_FadeOutObj;

public:
    virtual void begin() override;

public : 
    virtual void LevelEnd() override;
    virtual void LevelExit() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CFlowMgr_LvStart);
    CFlowMgr_LvStart();
    CFlowMgr_LvStart(const CFlowMgr_LvStart& _Origin);
    virtual ~CFlowMgr_LvStart();
};
