#pragma once
#include <Engine\\CScript.h>

class CGameObject;

class CLensFlareScript : public CScript
{
private:
    bool m_bDepthCheck;
    float m_Spacing;
    Vec2 m_CenterOffset;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    Vec4 GetSunNDCPos();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CLensFlareScript);

public:
    CLensFlareScript();
    CLensFlareScript(const CLensFlareScript& origin);
    virtual ~CLensFlareScript();
};
