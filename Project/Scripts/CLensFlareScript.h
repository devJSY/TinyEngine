#pragma once
#include <Engine\\CScript.h>

class CGameObject;

class CLensFlareScript : public CScript
{
private:
    Vec3 m_SunWorldPos;
    float m_Spacing;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    Vec2 GetSunNDCPos();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CLensFlareScript);

public:
    CLensFlareScript();
    CLensFlareScript(const CLensFlareScript& origin);
    virtual ~CLensFlareScript();
};
