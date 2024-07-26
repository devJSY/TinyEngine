#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CAnimator.h"

CTransform::CTransform()
    : CComponent(COMPONENT_TYPE::TRANSFORM)
    , m_LocalPos(Vec3())
    , m_LocalRotation(Vec3())
    , m_LocalQuaternion()
    , m_LocalScale(Vec3(1.f, 1.f, 1.f))
    , m_arrLocalDir{}
    , m_arrWorldDir{}
    , m_matWorld()
    , m_matWorldInv()
    , m_Mobility(MOBILITY_TYPE::MOVABLE)
    , m_bAbsolute(true)
    , m_matTransformation()
{
}

CTransform::CTransform(const CTransform& origin)
    : CComponent(origin)
    , m_LocalPos(origin.m_LocalPos)
    , m_LocalRotation(origin.m_LocalRotation)
    , m_LocalQuaternion(origin.m_LocalQuaternion)
    , m_LocalScale(origin.m_LocalScale)
    , m_arrLocalDir{origin.m_arrLocalDir[0], origin.m_arrLocalDir[1], origin.m_arrLocalDir[2]}
    , m_arrWorldDir{origin.m_arrWorldDir[0], origin.m_arrWorldDir[1], origin.m_arrWorldDir[2]}
    , m_matWorld()
    , m_matWorldInv()
    , m_Mobility(origin.m_Mobility)
    , m_bAbsolute(origin.m_bAbsolute)
    , m_matTransformation()
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
    m_matWorld = XMMatrixIdentity();

    Matrix matScale = XMMatrixScaling(m_LocalScale.x, m_LocalScale.y, m_LocalScale.z);

    m_LocalQuaternion = Quat::CreateFromAxisAngle(Vec3(1.f, 0.f, 0.f), m_LocalRotation.x) *
                        Quat::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), m_LocalRotation.y) *
                        Quat::CreateFromAxisAngle(Vec3(0.f, 0.f, 1.f), m_LocalRotation.z);

    Matrix matRot = Matrix::CreateFromQuaternion(m_LocalQuaternion);

    Matrix matTranslation = XMMatrixTranslation(m_LocalPos.x, m_LocalPos.y, m_LocalPos.z);

    m_matWorld = matScale * matRot * matTranslation;

    // 물체의 방향값을 다시 계산한다.
    static const Vec3 BasisVector[3] = {Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f)};

    // Vec3 를 Vec4 타입으로 확장해서 행렬을 적용시켜야 함
    // XMVector3TransformCoord	- w 를 1로 확장
    // XMVector3TransformNormal - w 를 0으로 확장
    // mul(float4(_in.vPos, 1 or 0), g_matWorld);
    // 적용 받을 상태행렬의 이동을 적용할지 말지 결정
    for (int i = 0; i < 3; ++i)
    {
        // m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
        m_arrLocalDir[i] = XMVector3TransformNormal(BasisVector[i], m_matWorld);
        m_arrWorldDir[i] = m_arrLocalDir[i].Normalize();
    }

    // 부모 오브젝트가 있다면
    CGameObject* pParent = GetOwner()->GetParent();
    if (nullptr != pParent)
    {
        // Bone Socket Matrix
        Matrix SocketMat = XMMatrixIdentity();
        Matrix BoneTransformMat = XMMatrixIdentity();

        const tBoneSocket* pBoneSocket = GetOwner()->GetBoneSocket();
        if (nullptr != pBoneSocket && nullptr != pParent->Animator() && pParent->Animator()->IsValid())
        {
            SocketMat = pBoneSocket->matSocket;
            BoneTransformMat = pParent->Animator()->GetBoneTransformMat(pBoneSocket->BoneIndex);
        }

        // Parent Matrix = Socket Matrix * Bone Transform Matrix * Parent World
        m_matTransformation = SocketMat * BoneTransformMat * pParent->Transform()->GetWorldMat();

        if (m_bAbsolute)
        {
            Vec3 Translation, Rotation, Scale;
            ImGuizmo::DecomposeMatrixToComponents(*m_matTransformation.m, Translation, Rotation, Scale);
            Matrix matParentScaleInv = XMMatrixScaling(1.f / Scale.x, 1.f / Scale.y, 1.f / Scale.z);

            // 부모의 크기 행렬 상쇄
            m_matTransformation = matParentScaleInv * m_matTransformation;
        }

        // 변환행렬 적용
        m_matWorld *= m_matTransformation;

        // 부모 변환 행렬이 적용된 월드 행렬로 방향 재계산
        for (int i = 0; i < 3; ++i)
        {
            m_arrWorldDir[i] = XMVector3TransformNormal(BasisVector[i], m_matWorld);
            m_arrWorldDir[i].Normalize();
        }
    }

    m_matWorldInv = m_matWorld.Invert();
}

void CTransform::UpdateData()
{
    g_Transform.matWorld = m_matWorld;
    g_Transform.matWorldInv = m_matWorldInv;

    g_Transform.matWorldInvTranspose = g_Transform.matWorld;
    g_Transform.matWorldInvTranspose.Translation(Vec3(0.0f));
    g_Transform.matWorldInvTranspose = g_Transform.matWorldInvTranspose.Transpose().Invert();

    g_Transform.matWV = g_Transform.matWorld * g_Transform.matView;
    g_Transform.matWVP = g_Transform.matWV * g_Transform.matProj;

    // 위치정보를 Transform 상수버퍼에 보내고, B0 레지스터에 바인딩
    CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
    pCB->SetData(&g_Transform);
    pCB->UpdateData();
}

void CTransform::SetDirection(Vec3 _Forward)
{
    if (_Forward == Vec3::Zero)
        return;

    _Forward.Normalize();
    Vec3 Up = Vec3(0.f, 1.f, 0.f);

    Vec3 Right = Up.Cross(_Forward);
    Right.Normalize();

    Up = _Forward.Cross(Right);
    Up.Normalize();

    Matrix rotationMatrix = Matrix();
    rotationMatrix.Forward(-_Forward);
    rotationMatrix.Up(Up);
    rotationMatrix.Right(Right);

    Quat Rot = Quat::CreateFromRotationMatrix(rotationMatrix);
    Transform()->SetWorldRotation(Rot);
}

void CTransform::Slerp(Vec3 _TowardDir, float _t)
{
    _TowardDir.Normalize();
    // FrontDir 와 TowardDir 가 비슷한 방향이면 보간 X
    if (_TowardDir == Vec3::Zero || _TowardDir.Dot(GetWorldDir(DIR_TYPE::FRONT)) >= 1.f - 1e-5f)
        return;

    // 회전 축 정렬
    Vec3 Up = Vec3(0.f, 1.f, 0.f);

    Vec3 Right = Up.Cross(_TowardDir);
    Right.Normalize();

    Up = _TowardDir.Cross(Right);
    Up.Normalize();

    // 회전 행렬 생성
    Matrix RotationMatrix = Matrix();
    RotationMatrix.Forward(-_TowardDir);
    RotationMatrix.Up(Up);
    RotationMatrix.Right(Right);

    // Slerp
    Quat SlerpQuat = Quat::Slerp(GetWorldQuaternion(), Quat::CreateFromRotationMatrix(RotationMatrix), _t);

    // Slerp 계산된 쿼터니언 으로부터 회전 행렬 생성
    RotationMatrix = Matrix::CreateFromQuaternion(SlerpQuat);

    // 회전 행렬의 Forward Vector 추출
    Vec3 LookDir = -RotationMatrix.Forward();

    // 방향 설정
    SetDirection(LookDir);
}

Quat CTransform::GetWorldQuaternion() const
{
    Vec3 Translation, Rotation, Scale;
    ImGuizmo::DecomposeMatrixToComponents(*m_matWorld.m, Translation, Rotation, Scale);
    Rotation.ToRadian();

    return Quat::CreateFromAxisAngle(Vec3(1.f, 0.f, 0.f), Rotation.x) * Quat::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), Rotation.y) *
           Quat::CreateFromAxisAngle(Vec3(0.f, 0.f, 1.f), Rotation.z);
}

Vec3 CTransform::GetWorldScale() const
{
    Vec3 Translation, Rotation, Scale;
    ImGuizmo::DecomposeMatrixToComponents(*m_matWorld.m, Translation, Rotation, Scale);
    return Scale;
}

void CTransform::SetWorldScale(Vec3 _Scale)
{
    m_bAbsolute = true;
    SetLocalScale(_Scale);
}

Vec3 CTransform::GetTransformWorldScale() const
{
    CGameObject* pParent = GetOwner()->GetParent();
    Vec3 vWorldScale = m_LocalScale;

    bool bAbsolute = m_bAbsolute;

    while (pParent)
    {
        if (!bAbsolute)
        {
            vWorldScale *= pParent->Transform()->GetLocalScale();
        }

        bAbsolute = pParent->Transform()->IsAbsolute();
        pParent = pParent->GetParent();
    }

    return vWorldScale;
}

void CTransform::SetWorldPos(Vec3 _Pos)
{
    SetLocalPos(_Pos - GetWorldPos() + m_LocalPos);
}

void CTransform::SetLocalRotation(Vec3 _Radian)
{
    m_LocalRotation = _Radian;
    finaltick(); // Dir 재계산
}

Vec3 CTransform::GetWorldRotation() const
{
    Vec3 Translation, Rotation, Scale;
    ImGuizmo::DecomposeMatrixToComponents(*m_matWorld.m, Translation, Rotation, Scale);
    Rotation.ToRadian();
    return Rotation;
}

void CTransform::SetWorldRotation(Vec3 _Radian)
{
    CGameObject* pParent = GetOwner()->GetParent();
    if (nullptr != pParent)
    {
        Vec3 ParentWorldRot = pParent->Transform()->GetWorldRotation();
        _Radian -= ParentWorldRot;
    }

    SetLocalRotation(_Radian);
}

UINT CTransform::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_LocalPos, sizeof(Vec3), 1, _File);
    fwrite(&m_LocalRotation, sizeof(Vec3), 1, _File);
    fwrite(&m_LocalScale, sizeof(Vec3), 1, _File);
    fwrite(&m_Mobility, sizeof(MOBILITY_TYPE), 1, _File);
    fwrite(&m_bAbsolute, sizeof(bool), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(MOBILITY_TYPE);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CTransform::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_LocalPos, sizeof(Vec3), 1, _File);
    fread(&m_LocalRotation, sizeof(Vec3), 1, _File);
    fread(&m_LocalScale, sizeof(Vec3), 1, _File);
    fread(&m_Mobility, sizeof(MOBILITY_TYPE), 1, _File);
    fread(&m_bAbsolute, sizeof(bool), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(MOBILITY_TYPE);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}
