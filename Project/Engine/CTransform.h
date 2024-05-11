#pragma once
#include "CComponent.h"

class CTransform : public CComponent
{
private:
    Vec3 m_vRelativePos;
    Vec3 m_vRelativeScale;
    Vec3 m_vRelativeRotation;

    Vec3 m_arrLocalDir[3]; // Right, Up, Front
    Vec3 m_arrWorldDir[3]; // Right, Up, Front

    Matrix m_matWorld;

    MOBILITY_TYPE m_Mobility;
    bool m_bAbsolute;

    Matrix m_matTransformation;

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

    Vec3 GetWorldPos() const { return m_matWorld.Translation(); }
    Vec3 GetWorldScale() const;
    Vec3 GetWorldRotation() const;

    void SetDirection(Vec3 _Dir);

    MOBILITY_TYPE GetMobilityType() const { return m_Mobility; }
    void SetMobilityType(MOBILITY_TYPE _Type) { m_Mobility = _Type; }

    bool IsAbsolute() const { return m_bAbsolute; }
    void SetAbsolute(bool _bAbsolute) { m_bAbsolute = _bAbsolute; }

    const Matrix& GetWorldMat() const { return m_matWorld; }
    void SetWorldMat(const Matrix _matWorld) { m_matWorld = _matWorld; }

    Vec3 GetLocalDir(DIR_TYPE _type) const { return m_arrLocalDir[(UINT)_type]; }
    Vec3 GetWorldDir(DIR_TYPE _type) const { return m_arrWorldDir[(UINT)_type]; }

public:
    const Matrix& GetTransformationMat() const { return m_matTransformation; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CTransform)
public:
    CTransform();
    CTransform(const CTransform& origin);
    virtual ~CTransform();
};
