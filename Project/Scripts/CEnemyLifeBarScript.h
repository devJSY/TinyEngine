#pragma once
#include <Engine\\CScript.h>

class CEnemyScript;

class CEnemyLifeBarScript : public CScript
{
private:
    CEnemyScript* m_Enemy;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetEnemy(CEnemyScript* _pEnemy) { m_Enemy = _pEnemy; };

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CEnemyLifeBarScript);

public:
    CEnemyLifeBarScript();
    virtual ~CEnemyLifeBarScript();
};
