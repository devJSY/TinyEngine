#pragma once
#include <Engine\\CScript.h>

class CLevelChangeScript : public CScript
{
public:
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CLevelChangeScript);

public:
    CLevelChangeScript();
    virtual ~CLevelChangeScript();
};
