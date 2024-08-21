#pragma once
#include "CLevelFlowMgr.h"

enum class Lv4State
{
    Idle,
    LightOut,
    End,
};

class CFlowMgr_Lv4 : public CLevelFlowMgr
{
private:
    CLight* m_pLight[2];
    Lv4State m_eState;

    float m_fRatio;
    float m_fLightOutTime;
    float m_fAccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void TriggerEvent(UINT _Idx);

public:
    virtual void LevelEnd() override;

private:
    void Idle();
    void LightOut();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CFlowMgr_Lv4);
    CFlowMgr_Lv4();
    CFlowMgr_Lv4(const CFlowMgr_Lv4& _Origin);
    virtual ~CFlowMgr_Lv4();
};
