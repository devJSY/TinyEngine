#pragma once
#include <Engine\\CScript.h>

class CMomentaryObjScript : public CScript
{
private:
    Vec3 m_InitVelocity;
    float m_PlayTime;
    double m_EndAnimPlayTime;
    wstring m_EndAnim;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetInitVelocity(Vec3 _Vel) { m_InitVelocity = _Vel; }
    void SetPlayTime(float _Time) { m_PlayTime = _Time; }
    void SetEndAnim(wstring _Key);

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

public:
    CLONE(CMomentaryObjScript);
    CMomentaryObjScript();
    virtual ~CMomentaryObjScript();
};