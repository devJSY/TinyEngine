#pragma once
#include <Engine\\CScript.h>

class CFXScript : public CScript
{
public:
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CFXScript)

public:
    CFXScript();
    virtual ~CFXScript();
};
