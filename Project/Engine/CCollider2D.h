#pragma once
#include "CComponent.h"

class CCollider2D : public CComponent
{
protected:
    void* m_RuntimeFixture;

    Vec2 m_Offset;
    bool m_bTrigger;
    float m_Friction;
    float m_Bounciness;

    int m_CollisionCount;

private:
    Vec3 m_PrevScale;

public:
    virtual void finaltick() override;

public:
    Vec2 GetOffset() const { return m_Offset; }
    void SetOffset(Vec2 _offset);

    bool IsTrigger() const { return m_bTrigger; }
    void SetTrigger(bool _trigger);

    float GetFriction() const { return m_Friction; }
    void SetFriction(float _Friction);

    float GetBounciness() const { return m_Bounciness; }
    void SetBounciness(float _Bounciness);

private:
    void OnCollisionEnter(CCollider2D* _OtherCollider);
    void OnCollisionStay(CCollider2D* _OtherCollider);
    void OnCollisionExit(CCollider2D* _OtherCollider);

    void OnTriggerEnter(CCollider2D* _OtherCollider);
    void OnTriggerStay(CCollider2D* _OtherCollider);
    void OnTriggerExit(CCollider2D* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual CCollider2D* Clone() = 0;

public:
    CCollider2D(COMPONENT_TYPE _Type);
    virtual ~CCollider2D();

    friend class CPhysics2DMgr;
    friend class CollisionCallback;
};
