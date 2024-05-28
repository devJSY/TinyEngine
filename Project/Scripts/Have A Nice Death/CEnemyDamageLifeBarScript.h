#pragma once
#include <Engine\\CScript.h>

class CEnemyScript;

class CEnemyDamageLifeBarScript : public CScript
{
private:
    CEnemyScript* m_Enemy;
    float m_RenderLife;
    float m_DelayTime;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    void TakeHit(float _DelayTime) { m_DelayTime = _DelayTime; }
    void SetEnemy(CEnemyScript* _pEnemy) { m_Enemy = _pEnemy; };

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CEnemyDamageLifeBarScript);

public:
    CEnemyDamageLifeBarScript();
    virtual ~CEnemyDamageLifeBarScript();
};
