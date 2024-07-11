#pragma once
#include "CComponent.h"

class CTransform : public CComponent
{
private:
    Vec3 m_LocalPos;
    Vec3 m_LocalRotation;
    Quat m_LocalQuaternion;
    Vec3 m_LocalScale;

    Vec3 m_arrLocalDir[3]; // Right, Up, Front
    Vec3 m_arrWorldDir[3]; // Right, Up, Front

    Matrix m_matWorld;
    Matrix m_matWorldInv;

    MOBILITY_TYPE m_Mobility;
    bool m_bAbsolute;

    Matrix m_matTransformation;

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;

public:
    Vec3 GetLocalPos() const { return m_LocalPos; }
    void SetLocalPos(Vec3 _Pos) { m_LocalPos = _Pos; }
    Vec3 GetWorldPos() const { return m_matWorld.Translation(); }
    void SetWorldPos(Vec3 _Pos);

    Vec3 GetLocalRotation() const { return m_LocalRotation; }
    void SetLocalRotation(Vec3 _Radian);
    Vec3 GetWorldRotation() const;
    void SetWorldRotation(Vec3 _Radian);

    Quat GetLocalQuaternion() const { return m_LocalQuaternion; }
    void SetDirection(Vec3 _Dir);
    Quat GetWorldQuaternion() const;

    Vec3 GetLocalScale() const { return m_LocalScale; }
    void SetLocalScale(Vec3 _Scale) { m_LocalScale = _Scale; }
    Vec3 GetWorldScale() const;
    void SetWorldScale(Vec3 _Scale);
    Vec3 GetTransformWorldScale() const;

    Vec3 GetLocalDir(DIR_TYPE _type) const { return m_arrLocalDir[(UINT)_type]; }
    Vec3 GetWorldDir(DIR_TYPE _type) const { return m_arrWorldDir[(UINT)_type]; }

    const Matrix& GetWorldMat() const { return m_matWorld; }
    const Matrix& GetWorldInvMat() const { return m_matWorldInv; }
    void SetWorldMat(const Matrix _matWorld) { m_matWorld = _matWorld; }

    MOBILITY_TYPE GetMobilityType() const { return m_Mobility; }
    void SetMobilityType(MOBILITY_TYPE _Type) { m_Mobility = _Type; }

    bool IsAbsolute() const { return m_bAbsolute; }
    void SetAbsolute(bool _bAbsolute) { m_bAbsolute = _bAbsolute; }

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
