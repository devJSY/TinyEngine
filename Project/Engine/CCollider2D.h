#pragma once
#include "CComponent.h"
#include "CPhysics2DMgr.h"

class CCollider2D : public CComponent
{
protected:
    void* m_RuntimeFixture;

    Ptr<CPhysics2DMaterial> m_Mtrl;
    bool m_bTrigger;
    Vec2 m_Offset;

    int m_CollisionCount;
    int m_TriggerCount;

protected:
    Vec3 m_PrevScale;
    bool m_bEnabled; // Rigidbody를 보유한경우 사용하지않음

public:
    virtual void begin() override;
    virtual void finaltick() override;

public:
    Ptr<CPhysics2DMaterial> GetMaterial() const { return m_Mtrl; }
    void SetMaterial(Ptr<CPhysics2DMaterial> _Mtrl);

    bool IsTrigger() const { return m_bTrigger; }
    void SetTrigger(bool _trigger);

    Vec2 GetOffset() const { return m_Offset; }
    void SetOffset(Vec2 _offset);

public:
    bool IsEnabled();
    void SetEnabled(bool _bEnabled);

public:
    bool IsCollision(Vec2 _Point);
    bool RayCast(Vec2 _Origin, Vec2 _Dirction, float _Distance, RaycastHit2D& _Hit);

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
    CCollider2D(const CCollider2D& origin);
    virtual ~CCollider2D();

    friend class CPhysics2DMgr;
    friend class CCollision2DCallback;
};
