#pragma once
#include <Engine\\CScript.h>

class CPlayerLifeTextScript : public CScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CPlayerLifeTextScript);

public:
    CPlayerLifeTextScript();
    virtual ~CPlayerLifeTextScript();
};
