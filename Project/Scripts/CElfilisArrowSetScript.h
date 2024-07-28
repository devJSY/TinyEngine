#pragma once
#include <Engine\\CScript.h>

class CElfilisArrowSetScript : public CScript
{
private:
    CGameObject* m_Target;
    UINT m_Step;
    float m_Speed;
    float m_AccTime;
    bool m_bArrowStart[7];

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetTarget(CGameObject* _Target) { m_Target = _Target; }

    bool IsSpawnFinish() { return m_Step != 0; }

private:
    void Spawn();
    void Ready();
    void Fly();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisArrowSetScript);
    CElfilisArrowSetScript();
    virtual ~CElfilisArrowSetScript();
};