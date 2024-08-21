#pragma once

#include "CLevelFlowMgr.h"

class CFlowMgr_Lv2 : public CLevelFlowMgr
{
private:
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void LevelStart() override;
    virtual void LevelEnd() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CFlowMgr_Lv2);
    CFlowMgr_Lv2();
    CFlowMgr_Lv2(const CFlowMgr_Lv2& _Origin);
    virtual ~CFlowMgr_Lv2();

};
