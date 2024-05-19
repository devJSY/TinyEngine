#pragma once
#include <Engine\\CScript.h>

class CBulletScript : public CScript
{
private:
    Vec3 m_InitVelocity;

public:
    void SetInitVelocity(Vec3 _Vel) { m_InitVelocity = _Vel; }

public:
    virtual void begin() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CBulletScript);

public:
    CBulletScript();
    virtual ~CBulletScript();
};
