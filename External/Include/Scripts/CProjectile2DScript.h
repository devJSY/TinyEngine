#pragma once
#include <Engine\\CScript.h>

class CProjectile2DScript : public CScript
{
private:
    Vec3 m_vDir;
    float m_Speed;

private:
    virtual void tick() override;

public:
    void SetTarget(CGameObject* _pTarget, Vec3 _vBaseDir);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CProjectile2DScript);

public:
    CProjectile2DScript();
    CProjectile2DScript(const CProjectile2DScript& origin);
    virtual ~CProjectile2DScript();
};
