#pragma once

#include "CLevelFlowMgr.h"

class CFlowMgr_LvStart : public CLevelFlowMgr
{
private:
    static bool m_bEnter;

public:
    virtual void begin() override;
    
public:
    virtual void LevelStart() override;
    virtual void LevelEnd() override;
    virtual void LevelRestart() override;
    virtual void RobbyLevel() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CFlowMgr_LvStart);
    CFlowMgr_LvStart();
    CFlowMgr_LvStart(const CFlowMgr_LvStart& _Origin);
    virtual ~CFlowMgr_LvStart();
};
