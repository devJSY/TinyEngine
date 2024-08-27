#pragma once
#include <Engine\CScript.h>

enum class CUIBurstImpactEffectState
{
    Enter,
    End,
};

class CUIBurstImpactEffectScript : public CScript
{
private:
    CUIBurstImpactEffectState m_eState;
    float m_fScale;

public:
    virtual void tick() override;

public:
    float GetScale() const { return m_fScale; }
    void ChangeState(CUIBurstImpactEffectState _eState) { m_eState = _eState; }

private:
    void Enter();
    void End();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIBurstImpactEffectScript)
public:
    CUIBurstImpactEffectScript();
    CUIBurstImpactEffectScript(const CUIBurstImpactEffectScript& Origin);
    virtual ~CUIBurstImpactEffectScript();
};
