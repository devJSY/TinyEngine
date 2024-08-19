#pragma once
#include <Engine\\CScript.h>

class CFollowPlayerScript : public CScript
{
private:
    Vec3 m_LightDir;
    float m_Dist;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CFollowPlayerScript);
    CFollowPlayerScript();
    CFollowPlayerScript(const CFollowPlayerScript& _Origin);
    virtual ~CFollowPlayerScript();
};