#pragma once
#include <Engine\CScript.h>
class CUIMtrlScript : public CScript
{
private:
    Vec2 m_vPatternUV1;
    Vec2 m_vPatternUV2;
    float m_fSpeed1;
    float m_fSpeed2;

public:
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIMtrlScript);

public:
    CUIMtrlScript();
    virtual ~CUIMtrlScript();
};
