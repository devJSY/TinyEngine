#pragma once
#include <Engine\\CScript.h>

class CEnemyScript;

class CEnemyHitBoxScript : public CScript
{
private:
    CEnemyScript* m_Enemy;

public:
    virtual void begin() override;

public:
    void SetEnemy(CEnemyScript* _pEnemy) { m_Enemy = _pEnemy; };
    void SetEnable(bool _bEnable);

public:
    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CEnemyHitBoxScript);

public:
    CEnemyHitBoxScript();
    virtual ~CEnemyHitBoxScript();
};
