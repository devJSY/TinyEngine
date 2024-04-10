#pragma once
#include <Engine\\CScript.h>

class CProjectile2DScript : public CScript
{
private:
    CGameObject* m_pTarget;
    Vec3 m_vBaseDir;
    Vec3 m_vInitDir;
    float m_Force;
    int m_ATK;
    float m_Duration;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetTarget(CGameObject* _pTarget, Vec3 _vBaseDir);
    void ZAxisAlign(Vec3 _vToDir, Vec3 _vBaseDir);
    void SetInitDirection(Vec3 _Dir) { m_vInitDir = _Dir; }

private:
    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CProjectile2DScript);

public:
    CProjectile2DScript();
    CProjectile2DScript(const CProjectile2DScript& origin);
    virtual ~CProjectile2DScript();
};
