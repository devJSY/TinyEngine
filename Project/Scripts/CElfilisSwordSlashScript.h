#pragma once
#include <Engine\\CScript.h>

class CElfilisSwordSlashScript : public CScript
{
private:
    Vec3 m_InitDir;
    float m_AccTime;
    float m_LifeTime;
    bool m_bSpawnDropStar;

public:
    virtual void begin() override;
    virtual void tick() override;

    public:
    void SetInitDir(Vec3 _Dir) { m_InitDir.y = 0.f; m_InitDir = _Dir.Normalize(); }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisSwordSlashScript);
    CElfilisSwordSlashScript();
    virtual ~CElfilisSwordSlashScript();
};
