#include "pch.h"
#include "CCameraController.h"

#include <Engine/CLevelMgr.h>

CCameraController::CCameraController()
    : CScript(CAMERACONTROLLER)
    , m_Target(nullptr)
    , m_LookDir(0.f, 0.f, 0.f)
    , m_LookDist(0.f)
    , m_RotationSpeed(50.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_LookDir, "Init Look Dir", 0.05f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_LookDist, "Init Look Dis", 0.05f);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinSpeed, "m_MinSpeed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxSpeed, "m_MaxSpeed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ThresholdDistance, "m_ThresholdDistance");
}

CCameraController::~CCameraController()
{
}

Quat VectorToQuaternion(Vec3 _v)
{
    _v.Normalize();
    Vec3 Up = Vec3(0.f, 1.f, 0.f);

    Vec3 Right = Up.Cross(_v);
    Right.Normalize();

    Up = _v.Cross(Right);
    Up.Normalize();

    Matrix rotationMatrix = Matrix();
    rotationMatrix.Forward(-_v);
    rotationMatrix.Up(Up);
    rotationMatrix.Right(Right);

    return Quat::CreateFromRotationMatrix(rotationMatrix);
}

Vec3 QuaternionToVector(const Quat& quaternion)
{
    Matrix rotationMatrix = Matrix::CreateFromQuaternion(quaternion);
    return {rotationMatrix._31, rotationMatrix._32, rotationMatrix._33};
}

void CCameraController::begin()
{
    // Level Begin시 플레이어를 타겟으로 지정한다.
    if (m_Target == nullptr)
    {
        CLevel* CurrentLevel = CLevelMgr::GetInst()->GetCurrentLevel();
        m_Target = CurrentLevel->FindObjectByName(L"Main Player", LAYER_PLAYER);

        // Player가 없는 경우 Assert
        assert(m_Target);
    }

    // FOV 설정
    if (nullptr != Camera())
    {
        Camera()->SetFOV(XM_PI / 4.f);
    }

    // 카메라 세팅
    Vec3 InitPos = CalCamPos(m_Target->Transform()->GetWorldPos(), m_LookDir, m_LookDist);

    m_LookEyePos = InitPos;

    m_PrevCamPos = InitPos;
    m_PrevLookDir = m_LookDir;

    m_CurCamPos = InitPos;
    m_CurLookDir = m_LookDir;

    Transform()->SetWorldPos(InitPos);
    Transform()->SetDirection(m_LookDir);
}

void CCameraController::tick()
{
    // Test
    if (KEY_TAP(KEY::G) || KEY_PRESSED(KEY::G))
    {
        m_LookDir = Vec3(1.f, -1.f, 0.f);
        m_LookDir.Normalize();
    }

    if (KEY_TAP(KEY::H) || KEY_PRESSED(KEY::H))
    {
        m_LookDir = Vec3(0.f, -1.f, 1.f);
        m_LookDir.Normalize();
    }

    if (KEY_TAP(KEY::J) || KEY_PRESSED(KEY::J))
    {
        m_LookDir = Vec3(-1.f, -1.f, 0.f);
        m_LookDir.Normalize();
    }

    // Target이 없다면 return
    if (nullptr == m_Target)
        return;

    // ========================= Prev Data Save =========================
    m_PrevCamPos = m_CurCamPos;
    m_PrevLookDir = m_CurLookDir;
    m_PrevLookAtPos = m_CurLookAtPos;

    // 타겟의 현재 위치 업데이트
    Vec3 TargetPos = m_Target->Transform()->GetWorldPos();

    // =========================== Pos Update ===========================
    Vec3 Diff = TargetPos - m_PrevLookAtPos;
    float MoveLength = Diff.Length();
    Vec3 MoveDir = Diff.Normalize();

    float Ratio = clamp((MoveLength / m_ThresholdDistance), 0.f, 1.f) * XM_PI / 2.f;
    float CamSpeed = m_MinSpeed + (m_MaxSpeed - m_MinSpeed) * sinf(Ratio);

    m_CurLookAtPos = m_PrevLookAtPos + MoveDir * CamSpeed * DT;

    // =========================== Dir Update ===========================
    // PrevLookDir과 LookDir사이의 각도 구하기
    m_PrevLookDir.Normalize();
    m_LookDir.Normalize();

    if (m_LookDir != m_PrevLookDir)
    {
        float Angle = acosf(clamp(m_PrevLookDir.Dot(m_LookDir), -1.f, 1.f));
        float degrees = Angle * (180.0f / XM_PI);

        if (m_LookDir.Dot(m_PrevLookDir) >= cosf(0.f) - 1e-5f)
        {
            m_CurLookDir = m_LookDir;
        }
        else
        {
            float MaxRotationStep = m_RotationSpeed * DT;
            float t = min(MaxRotationStep / degrees, 1.0f);

            Quat PrevQuat = Transform()->GetWorldQuaternion();
            Quat LookQuat = VectorToQuaternion(m_LookDir);

            Quat SlerpQuat = Quat::Slerp(PrevQuat, LookQuat, t);
            m_CurLookDir = QuaternionToVector(SlerpQuat);
        }
    }

    // =========================== 예외 처리 =============================
    // 보간 값이 목표값과 비슷하다면 그대로 세팅해준다.
    if ((TargetPos - m_CurLookAtPos).Length() <= 1e-6f)
    {
        m_CurLookAtPos = TargetPos;
    }

    // 카메라가 LookEyePos를 넘어서까지 이동했다면 CurPos를 LookEyePos로 세팅해준다.
    Vec3 LeftMoveDir = (TargetPos - m_CurLookAtPos).Normalize();
    if (MoveDir.Dot(LeftMoveDir) <= 0.f)
    {
        m_CurLookAtPos = TargetPos;
    }

    // ========================= Eye Pos Update =========================
    m_LookEyePos = CalCamPos(m_CurLookAtPos, m_CurLookDir, m_LookDist);

    // ==================== Camera Transform Update ====================
    Transform()->SetDirection(m_CurLookDir);
    Transform()->SetWorldPos(m_LookEyePos);

    // ==================== debug ========================
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), 200.f, Vec3(1.f, 0.f, 0.f), true);
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::RIGHT), 200.f, Vec3(0.f, 1.f, 0.f), true);
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::UP), 200.f, Vec3(0.f, 0.f, 1.f), true);
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), m_CurLookDir, m_LookDist, Vec3(0.f, 0.f, 0.f), true);
}

Vec3 CCameraController::CalCamPos(Vec3 _TargetWorldPos, Vec3 _LookDir, float _CamDist)
{
    _LookDir.Normalize();

    Vec3 RetVal = _TargetWorldPos - _LookDir * _CamDist;

    return RetVal;
}

float CCameraController::GetAngleBetweenVectors(Vec3 _V1, Vec3 _V2)
{
    _V1.Normalize();
    _V2.Normalize();

    _V1.Dot(_V2);

    return 0.0f;
}

UINT CCameraController::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_LookDir, sizeof(Vec3), 1, _File);
    fwrite(&m_LookDist, sizeof(float), 1, _File);

    fwrite(&m_MinSpeed, sizeof(float), 1, _File);
    fwrite(&m_MaxSpeed, sizeof(float), 1, _File);
    fwrite(&m_ThresholdDistance, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CCameraController::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_LookDir, sizeof(Vec3), 1, _File);
    fread(&m_LookDist, sizeof(float), 1, _File);

    fread(&m_MinSpeed, sizeof(float), 1, _File);
    fread(&m_MaxSpeed, sizeof(float), 1, _File);
    fread(&m_ThresholdDistance, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
