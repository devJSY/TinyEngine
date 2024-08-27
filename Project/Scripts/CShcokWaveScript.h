#pragma once
#include <Engine\\CScript.h>

class CShcokWaveScript : public CScript
{
private:
    float m_CircleSize;
    float m_CircleSizeSpeed;

public:
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CShcokWaveScript);

public:
    CShcokWaveScript();
    CShcokWaveScript(const CShcokWaveScript& origin);
    virtual ~CShcokWaveScript();
};
