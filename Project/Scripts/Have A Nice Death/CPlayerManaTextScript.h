#pragma once
#include <Engine\\CScript.h>

class CPlayerManaTextScript : public CScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CPlayerManaTextScript);

public:
    CPlayerManaTextScript();
    virtual ~CPlayerManaTextScript();
};
