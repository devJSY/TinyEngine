#pragma once
#include "CUnitScript.h"

enum class KirbyLightState
{
    NONE,
    TurningOn,
    TurningOff,
};

class CKirbyLightScript : public CScript
{
private:
    float m_OriginFallOfEnd;
    float m_TurningTime;
    float m_AccTime;
    KirbyLightState m_State;

public:
    virtual void begin() override;
    virtual void tick() override;
    void TurnOn();
    void TurnOff();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyLightScript)
    CKirbyLightScript();
    virtual ~CKirbyLightScript();
};
