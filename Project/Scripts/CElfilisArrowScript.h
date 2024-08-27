#pragma once
#include <Engine\\CScript.h>

class CElfilisArrowScript : public CScript
{
private:
    UINT m_Step;
    float m_AccTime;
    bool m_bSpawnDropStar;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void StartSpawn() { ChangeStep(1); };
    void StartAttack() { ChangeStep(2); };

private:
    // tick
    void Spawn();
    void Ready();
    void Attack();

    void ChangeStep(UINT _Step);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisArrowScript);
    CElfilisArrowScript();
    virtual ~CElfilisArrowScript();
};
