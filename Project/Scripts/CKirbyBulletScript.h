#pragma once
#include <Engine\\CScript.h>

class CKirbyBulletScript : public CScript
{
private:
    Vec3    m_InitVelocity;
    float   m_PlayTime;
    wstring m_EndAnim;
    double m_EndAnimPlayTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetInitVelocity(Vec3 _Vel) { m_InitVelocity = _Vel; }
    void SetPlayTime(float _Time) { m_PlayTime = _Time; }
    void SetEndAnim(wstring _Key);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyBulletScript);
    CKirbyBulletScript();
    virtual ~CKirbyBulletScript();
};
