#pragma once
#include <Engine\\CScript.h>

class CGameObject;

class CPlayerLifeBarScript : public CScript
{
private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CPlayerLifeBarScript);

public:
    CPlayerLifeBarScript();
    virtual ~CPlayerLifeBarScript();
};
