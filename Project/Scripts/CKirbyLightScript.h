#pragma once
#include "CUnitScript.h"

enum class KirbyLightState
{
    NONE,
    TurningOn,
    TurningOff,
    Drop,
};

class CKirbyLightScript : public CScript
{
private:
    float m_MinFallOfEnd;
    float m_MaxFallOfEnd;
    float m_FallRatio;
    float m_TurnOnTime;
    float m_TurnOffTime;
    float m_AccTime;
    KirbyLightState m_State;

public:
    virtual void begin() override;
    virtual void tick() override;

    void Init();
    void Drop();
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
