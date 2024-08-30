#pragma once
#include <Engine\CScript.h>
class CUIDashEffectScript : public CScript
{
private:
    float m_fRotateSpeed;

    float m_fRotateYSpeed;
    float m_fTextureValue;
    float m_fTextureValueSpeed;
    float m_fMaxThreshHold;

public:
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIDashEffectScript)
public:
    CUIDashEffectScript();
    CUIDashEffectScript(const CUIDashEffectScript& Origin);
    virtual ~CUIDashEffectScript();
};
