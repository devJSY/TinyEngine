#pragma once
#include "CComponent.h"

enum class COLLIDER2D_TYPE
{
    RECT,
    CIRCLE,
};

class CCollider2D : public CComponent
{
private:
    Vec3 m_vOffsetPos;
    Vec3 m_vOffsetScale;
    int m_CollisionCount; // 다른 충돌체와 충돌중인 횟수
    bool m_bAbsolute;

    Matrix m_matColWorld;
    COLLIDER2D_TYPE m_Type;

    float m_fRadius;

    BoundingBox m_BoundingBox;
    BoundingSphere m_BoundingSphere;

public:
    void SetAbsolute(bool _bAbsol) { m_bAbsolute = _bAbsol; }
    void SetOffsetPos(Vec2 _vOffset) { m_vOffsetPos = Vec3(_vOffset.x, _vOffset.y, 0.f); }
    void SetOffsetScale(Vec2 _vOffsetScale) { m_vOffsetScale = Vec3(_vOffsetScale.x, _vOffsetScale.y, 1.f); }
    void SetColliderType(COLLIDER2D_TYPE _Type);

    bool IsAbsolute() const { return m_bAbsolute; }
    Vec2 GetOffsetPos() const { return Vec2(m_vOffsetPos.x, m_vOffsetPos.y); }
    Vec2 GetOffsetScale() const { return Vec2(m_vOffsetScale.x, m_vOffsetScale.y); }
    COLLIDER2D_TYPE GetColliderType() const { return m_Type; }

    const Matrix& GetColliderWorldMat() const { return m_matColWorld; }

    float GetRadius() const { return m_fRadius; }
    void SetRadius(float radius) { m_fRadius = radius; }

    const BoundingBox& GetBoundingBox() const { return m_BoundingBox; }
    const BoundingSphere& GetBoundingSphere() const { return m_BoundingSphere; }

public:
    virtual void finaltick() override;

public:
    void BeginOverlap(CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CCollider2D();
    virtual ~CCollider2D();
};
