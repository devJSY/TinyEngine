#pragma once
#include <Engine\\CScript.h>

class CGameStartScript : public CScript
{
public:
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CGameStartScript);

public:
    CGameStartScript();
    virtual ~CGameStartScript();
};
