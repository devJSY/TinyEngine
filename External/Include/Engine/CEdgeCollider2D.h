#pragma once
#include "CCollider2D.h"

class CEdgeCollider2D : public CCollider2D
{
private:
    float m_EdgeRadius;
    Vec2 m_StartPoint;
    Vec2 m_EndPoint;

    bool m_bUseAdjacentStartPoint;
    bool m_bUseAdjacentEndPoint;
    Vec2 m_AdjacentStartPoint;
    Vec2 m_AdjacentEndPoint;

public:
    void SetEdgeRadius(float _Radius);
    float GetEdgeRadius() const { return m_EdgeRadius; }

    void SetStartPoint(Vec2 _Point);
    Vec2 GetStartPoint() const { return m_StartPoint; }

    void SetEndPoint(Vec2 _Point);
    Vec2 GetEndPoint() const { return m_EndPoint; }

    void SetUseAdjacentStartPoint(bool _Use);
    bool IsUseAdjacentStartPoint() const { return m_bUseAdjacentStartPoint; }

    void SetUseAdjacentEndPoint(bool _Use);
    bool IsUseAdjacentEndPoint() const { return m_bUseAdjacentEndPoint; }

    void SetAdjacentStartPoint(Vec2 _Point);
    Vec2 GetAdjacentStartPoint() const { return m_AdjacentStartPoint; }

    void SetAdjacentEndPoint(Vec2 _Point);
    Vec2 GetAdjacentEndPoint() const { return m_AdjacentEndPoint; }

public:
    virtual void finaltick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CEdgeCollider2D);

public:
    CEdgeCollider2D();
    virtual ~CEdgeCollider2D();

    friend class CPhysics2DMgr;
};
