#pragma once
#pragma once
#include "CComponent.h"

class CTransform : public CComponent
{
private:
    Vec3 m_LocalPos;
    Vec3 m_LocalRotation;
    Quat m_LocalQuaternion;
    Vec3 m_LocalScale;

    Vec3 m_WorldPos;
    Vec3 m_WorldRotation;
    Quat m_WorldQuaternion;
    Vec3 m_WorldScale;

    Vec3 m_arrLocalDir[3]; // Right, Up, Front
    Vec3 m_arrWorldDir[3]; // Right, Up, Front

    Matrix m_matPrevWorld;
    Matrix m_matWorld;
    Matrix m_matWorldInv;

    MOBILITY_TYPE m_Mobility;
    bool m_bAbsolute;

    Matrix m_matTransformation;

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;

public:
    void SetDirection(Vec3 _Forward);
    void Slerp(Vec3 _TowardDir, float _t);

public:
    Vec3 GetLocalPos() const { return m_LocalPos; }
    void SetLocalPos(Vec3 _Pos) { m_LocalPos = _Pos; }
    Vec3 GetWorldPos() const { return m_WorldPos; }
    void SetWorldPos(Vec3 _Pos);

    Vec3 GetLocalRotation() const { return m_LocalRotation; }
    void SetLocalRotation(Vec3 _Radian);
    void SetLocalRotation(Quat _Quaternion) { SetLocalRotation(_Quaternion.ToEuler()); };

    Vec3 GetWorldRotation() const { return m_WorldRotation; }
    void SetWorldRotation(Vec3 _Radian);
    void SetWorldRotation(Quat _Quaternion) { SetWorldRotation(_Quaternion.ToEuler()); };

    Quat GetLocalQuaternion() const { return m_LocalQuaternion; }
    Quat GetWorldQuaternion() const { return m_WorldQuaternion; }

    Vec3 GetLocalScale() const { return m_LocalScale; }
    void SetLocalScale(Vec3 _Scale) { m_LocalScale = _Scale; }
    Vec3 GetWorldScale() const { return m_WorldScale; }
    void SetWorldScale(Vec3 _Scale);
    Vec3 GetTransformWorldScale() const;

    float GetWorldRatio() const { return (m_WorldScale.x + m_WorldScale.y + m_WorldScale.z) / 3.f; };

    Vec3 GetLocalDir(DIR_TYPE _type) const { return m_arrLocalDir[(UINT)_type]; }
    Vec3 GetWorldDir(DIR_TYPE _type) const { return m_arrWorldDir[(UINT)_type]; }

    const Matrix& GetWorldMat() const { return m_matWorld; }
    const Matrix& GetPrevWorldMat() const { return m_matPrevWorld; }
    const Matrix& GetWorldInvMat() const { return m_matWorldInv; }
    void SetWorldMat(const Matrix _matWorld) { m_matWorld = _matWorld; }

    MOBILITY_TYPE GetMobilityType() const { return m_Mobility; }
    void SetMobilityType(MOBILITY_TYPE _Type) { m_Mobility = _Type; }

    bool IsAbsolute() const { return m_bAbsolute; }
    void SetAbsolute(bool _bAbsolute) { m_bAbsolute = _bAbsolute; }

    const Matrix& GetTransformationMat() const { return m_matTransformation; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CTransform)
public:
    CTransform();
    CTransform(const CTransform& origin);
    virtual ~CTransform();
};
