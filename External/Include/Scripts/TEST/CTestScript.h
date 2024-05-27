#pragma once
#include <Engine\\CScript.h>

class CTestScript : public CScript
{
public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CTestScript);

public:
    CTestScript();
    virtual ~CTestScript();
};
