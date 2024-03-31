#pragma once
#include <Engine\\CScript.h>

class CProjectile2DScript : public CScript
{
private:
    CGameObject* m_pTarget;
    Vec3 m_vDir;
    float m_Speed;
    int m_ATK;
    float m_Duration;

private:
    virtual void tick() override;

public:
    void SetTarget(CGameObject* _pTarget, Vec3 _vBaseDir);

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
