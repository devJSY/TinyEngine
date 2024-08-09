#pragma once
#include <Engine\\CScript.h>

class CGameObject;

class CFadeOutScript : public CScript
{
private:
    CGameObject* pTarget;
    float m_Duration;
    float m_ElapsedTime;
    float m_RotateSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    Vec2 GetTargetNDCPos();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CFadeOutScript);

public:
    CFadeOutScript();
    CFadeOutScript(const CFadeOutScript& origin);
    virtual ~CFadeOutScript();
};
