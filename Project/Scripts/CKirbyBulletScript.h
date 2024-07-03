#pragma once
#include <Engine\\CScript.h>

class CKirbyBulletScript : public CScript
{
private:
    Vec3    m_InitVelocity;
    float   m_PlayTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetInitVelocity(Vec3 _Vel) { m_InitVelocity = _Vel; }
    void SetPlayTime(float _Time) { m_PlayTime = _Time; }

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

public:
    CLONE(CKirbyBulletScript);
    CKirbyBulletScript();
    virtual ~CKirbyBulletScript();
};
