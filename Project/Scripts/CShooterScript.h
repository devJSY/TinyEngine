#pragma once
#include <Engine\\CScript.h>

class CShooterScript : public CScript
{
private:
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CShooterScript);

public:
    CShooterScript();
    virtual ~CShooterScript();
};
