#pragma once
#include <Engine\\CScript.h>

class CProjectileScript : public CScript
{
private:
    Vec3 m_InitVelocity;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetInitVelocity(Vec3 _vel) { m_InitVelocity = _vel; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CProjectileScript);

public:
    CProjectileScript();
    virtual ~CProjectileScript();
};
