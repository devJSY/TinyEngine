#pragma once
#include <Engine\CScript.h>

enum class CommonHitEffectState
{
    Progress,
    End,
};

class CUICommonHitEffectScript : public CScript
{
private:
    Vec3 m_vOriginScale;

    CommonHitEffectState m_eState;
    float m_fLerpSpeed;
    float m_fRatio;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void Progress();
    void Stop();

    void Billboarding();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUICommonHitEffectScript)
public:
    CUICommonHitEffectScript();
    CUICommonHitEffectScript(const CUICommonHitEffectScript& Origin);
    virtual ~CUICommonHitEffectScript();
};
