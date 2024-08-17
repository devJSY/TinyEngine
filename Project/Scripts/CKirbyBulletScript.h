#pragma once
#include <Engine\\CScript.h>

class CKirbyBulletScript : public CScript
{
private:
    Vec3    m_MoveDir;
    Vec3    m_ChildOffest;
    float   m_ScaleFactor;
    float   m_Speed;
    float   m_PlayTime;
    bool    m_bHasTickCol;
    bool    m_bInit;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetInitVelocity(Vec3 _Vel);
    void SetPlayTime(float _Time) { m_PlayTime = _Time; }

private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider);
    virtual void OnTriggerEnter(CCollider* _OtherCollider);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyBulletScript);
    CKirbyBulletScript();
    virtual ~CKirbyBulletScript();
};
