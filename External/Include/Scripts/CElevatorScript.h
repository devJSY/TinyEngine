#pragma once
#include <Engine\\CScript.h>

class CElevatorScript : public CScript
{
private:
    bool m_IsPlayerSpawn;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CElevatorScript);

public:
    CElevatorScript();
    virtual ~CElevatorScript();
};
