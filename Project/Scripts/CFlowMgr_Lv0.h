#pragma once
#include "CLevelFlowMgr.h"

class CFlowMgr_Lv0 : public CLevelFlowMgr
{
private:


public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CFlowMgr_Lv0);
    CFlowMgr_Lv0();
    CFlowMgr_Lv0(const CFlowMgr_Lv0& _Origin);
    virtual ~CFlowMgr_Lv0();

};
