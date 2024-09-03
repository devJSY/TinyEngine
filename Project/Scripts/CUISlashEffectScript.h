#pragma once
#include <Engine\CScript.h>

enum class UISlashEffectState
{
    Progress,
    End,
};

class CUISlashEffectScript : public CScript
{
private:
    Vec3 m_vOriginScale;

    UISlashEffectState m_eState;
    float m_fRatio;
    float m_fLessRatio;
    float m_fLerpSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void Billboarding();
    void Progress();
    void Stop();

private:
    float RandomRotation();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUISlashEffectScript)
public:
    CUISlashEffectScript();
    CUISlashEffectScript(const CUISlashEffectScript& Origin);
    virtual ~CUISlashEffectScript();
};
