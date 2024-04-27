#pragma once
#include "CCollider2D.h"

class CPolygonCollider2D : public CCollider2D
{
private:
    vector<Vec2> m_Points; // 최대 정점 갯수 8개, 반시계방향, 볼록 다각형

public:
    void AddPoint(Vec2 _Point = Vec2());
    const vector<Vec2>& GetPoints() const { return m_Points; }
    void SetPoint(int _Idx, Vec2 _Point);
    void PointReSize(int _size);

public:
    virtual void begin() override;
    virtual void finaltick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPolygonCollider2D);

public:
    CPolygonCollider2D();
    virtual ~CPolygonCollider2D();

    friend class CPhysics2DMgr;
};
