#pragma once
#include <Engine\\CScript.h>

enum class MomentaryEffectType
{
    Scaling,
    DisappearAnim,
};

class CMomentaryObjScript : public CScript
{
private:
    deque<MomentaryEffectType> m_EffectVec;
    Vec3 m_InitVelocity;
    float m_AccTime;
    float m_LifeTime;

    // Effect : Scalling
    Vec3 m_OriginScale;
    Vec3 m_ScaleFactor;
    float m_ScaleTime;

    // Effect : Disappear Anim
    double m_EndAnimPlayTime;
    wstring m_EndAnim;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetInitVelocity(Vec3 _Vel) { m_InitVelocity = _Vel; }
    void SetPlayTime(float _Time) { m_LifeTime = _Time; }
    void AddEffect(MomentaryEffectType _EffectType);
    void SetEndAnim(wstring _Key);
    void SetScaling(Vec3 _ScaleFactor = Vec3(1.f), float _ScaleTime = 0.f);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CMomentaryObjScript);
    CMomentaryObjScript();
    virtual ~CMomentaryObjScript();
};