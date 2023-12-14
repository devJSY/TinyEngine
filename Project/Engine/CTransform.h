#pragma once
#include "CComponent.h"

class CTransform : public CComponent
{
private:
    Vec3 m_vRelativePos;
    Vec3 m_vRelativeScale;
    Vec3 m_vRelativeRotation;
    Vec3 m_arrDir[3]; // Right, Up, Front

    Matrix m_matWorld;

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;

public:
    void SetRelativePos(Vec3 _Pos) { m_vRelativePos = _Pos; }
    void SetRelativeScale(Vec3 _Scale) { m_vRelativeScale = _Scale; }
    void SetRelativeRotation(Vec3 _Rotation) { m_vRelativeRotation = _Rotation; }

    Vec3 GetRelativePos() const { return m_vRelativePos; }
    Vec3 GetRelativeScale() const { return m_vRelativeScale; }
    Vec3 GetRelativeRotation() const { return m_vRelativeRotation; }

    const Matrix& GetWorldMat() const { return m_matWorld; }

    Vec3 GetDir(DIR_TYPE _type) const { return m_arrDir[(UINT)_type]; }

public:
    CTransform();
    ~CTransform();
};
