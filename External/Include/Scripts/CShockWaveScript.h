#pragma once
#include <Engine\\CScript.h>

class CShockWaveScript : public CScript
{
private:
    float m_Time;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CShockWaveScript);

public:
    CShockWaveScript();
    virtual ~CShockWaveScript();
};
