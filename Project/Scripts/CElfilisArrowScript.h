#pragma once
#include <Engine\\CScript.h>

class CElfilisArrowScript : public CScript
{
private:
    UINT m_Step;
    float m_AccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void StartSpawn();
    void StartAttack() { m_Step = 2; m_AccTime = 0.f;}

private:
    void Spawn();
    void Ready();
    void Attack();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisArrowScript);
    CElfilisArrowScript();
    virtual ~CElfilisArrowScript();
};