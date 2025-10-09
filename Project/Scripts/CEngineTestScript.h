#pragma once
#include <Engine/CScript.h>

class CEngineTestScript : public CScript
{
private:
    float TestParam1 = 1.0f;
    int TestParam2 = 1;
    float TestParam3 = 1.0f;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void makeHeart();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CEngineTestScript);

public:
    CEngineTestScript();
    CEngineTestScript(const CEngineTestScript& origin);
    virtual ~CEngineTestScript() {}
};
