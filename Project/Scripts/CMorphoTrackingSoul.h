#pragma once
#include <Engine\\CScript.h>

class CMorphoTrackingSoul : public CScript
{
private:
    CGameObject* m_Target;
    StateStep m_Step;
    float m_Speed;
    float m_RotSpeed;
    float m_AccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;

    void Appear();
    void Tracking();
    void Disappear();

public:
    void SetTarget(CGameObject* _Target) { m_Target = _Target; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CMorphoTrackingSoul);
    CMorphoTrackingSoul();
    virtual ~CMorphoTrackingSoul();
};