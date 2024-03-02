#pragma once
#include <Engine\\CScript.h>

class CBulletScript : public CScript
{
private:
    Vec3 m_Dir;
    float m_Speed;
    float m_Duration;

public:
    void SetDir(Vec3 _dir) { m_Dir = _dir; }

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CBulletScript);

public:
    CBulletScript();
    virtual ~CBulletScript();
};
