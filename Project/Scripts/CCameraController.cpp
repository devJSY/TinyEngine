#include "pch.h"
#include "CCameraController.h"

#include <Engine/CLevelMgr.h>

CCameraController::CCameraController()
    : CScript(CAMERACONTROLLER)
    , m_Setup(CameraSetup::NORMAL)
    , m_Target(nullptr)
    , m_TargetPos(Vec3(0.f,0.f,0.f))
    , m_LookDir(0.f, 0.f, 0.f)
    , m_LookDist(0.f)
    , m_RotationSpeed(50.f)
    , m_EditRotSpeed(45.f)
    , m_EditZoomSpeed(300.f)
    , m_EditMode(false)
{

    AddScriptParam(SCRIPT_PARAM::VEC3, &m_Offset, "Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_LookDir, "Look Dir", 0.05f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_LookDist, "Look Distance", 1.f);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinSpeed, "MinSpeed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxSpeed, "MaxSpeed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ThresholdDistance, "Threshold Distance");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotationSpeed, "Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ZoomSpeed, "Zoom Speed");

    // Edit Mode
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_EditMode, "Edit Mode");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_EditRotSpeed, "Edit Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_EditZoomSpeed, "Edit Zoom Speed");
}

CCameraController::~CCameraController()
{
}

Quat VectorToQuaternion(Vec3 _v);
Vec3 QuaternionToVector(const Quat& quaternion);

void CCameraController::SetMainTarget(wstring _TargetName)
{
    CLevel* CurrentLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    m_Target = CurrentLevel->FindObjectByName(_TargetName);
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
    m_TargetPos = m_Target->Transform()->GetWorldPos();
    m_TargetPos += m_Offset;

    m_PrevLookDir = m_LookDir;
    m_CurLookDir = m_LookDir;
    m_PrevLookAtPos = m_TargetPos;
    m_CurLookAtPos = m_TargetPos;
    m_PrevDistance = m_LookDist;
    m_CurDistance = m_LookDist;

    Vec3 InitPos = CalCamPos(m_TargetPos, m_LookDir, m_LookDist);
    
    m_LookEyePos = InitPos;

    Transform()->SetWorldPos(InitPos);
    Transform()->SetDirection(m_LookDir);

    // Test
    m_ProgressStartPos = Vec3(0.f, 0.f, 0.f);
    m_ProgressStartDir = Vec3(0.f, -1.f, 1.f);
    m_ProgressStartDist = 1000.f;
    m_ProgressEndPos = Vec3(0.f, 0.f, 3000.f);
    m_ProgressEndDir = Vec3(-1.f, -1.f, 0.f);
    m_ProgressEndDist = 2000.f;

    m_Setup = CameraSetup::PROGRESS;
}

void CCameraController::tick()
{
    EditMode();

    // Target이 없다면 return
    if (nullptr == m_Target)
        return;

    // ========================= Prev Data Save =========================
    m_PrevLookDir = m_CurLookDir;
    m_PrevLookAtPos = m_CurLookAtPos;
    m_PrevDistance = m_CurDistance;

    // ========================= Target Update ==========================
    UpdateTargetPos();

    // 현재 Setup에 맞게 카메라의 LookDir, LookDist를 수정한다.
    SetUpProc();

    // LookDir, LookDist에 맞게 LookAtPos, LookDir을 업데이트한다.
    UpdateLookAtPos();
    UpdateLookDir();
    UpdateLookDistance();

    // ========================= Eye Pos Update =========================
    m_LookEyePos = CalCamPos(m_CurLookAtPos, m_CurLookDir, m_CurDistance);

    // ==================== Camera Transform Update ====================
    Transform()->SetDirection(m_CurLookDir);
    Transform()->SetWorldPos(m_LookEyePos);

    // ==================== debug ========================
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), 200.f, Vec3(1.f, 0.f, 0.f), true);
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::RIGHT), 200.f, Vec3(0.f, 1.f, 0.f), true);
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::UP), 200.f, Vec3(0.f, 0.f, 1.f), true);
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), m_CurLookDir, m_LookDist, Vec3(0.f, 0.f, 0.f), true);
}

void CCameraController::SetUpProc()
{
    switch (m_Setup)
    {
    case CameraSetup::NORMAL:
        break;
    case CameraSetup::PROGRESS:
        Progress();
        break;
    default:
        break;
    }
}

void CCameraController::UpdateTargetPos()
{
    // 타겟의 현재 위치 업데이트
    m_TargetPos = m_Target->Transform()->GetWorldPos();
    m_TargetPos += m_Offset;
}

void CCameraController::UpdateLookAtPos()
{
    // =========================== Pos Update ===========================
    Vec3 Diff = m_TargetPos - m_PrevLookAtPos;
    float MoveLength = Diff.Length();
    Vec3 MoveDir = Diff.Normalize();

    float Ratio = clamp((MoveLength / m_ThresholdDistance), 0.f, 1.f) * XM_PI / 2.f;
    float CamSpeed = m_MinSpeed + (m_MaxSpeed - m_MinSpeed) * sinf(Ratio);

    m_CurLookAtPos = m_PrevLookAtPos + MoveDir * CamSpeed * DT;

    // =========================== 예외 처리 =============================
    // 보간 값이 목표값과 비슷하다면 그대로 세팅해준다.
    if ((m_TargetPos - m_CurLookAtPos).Length() <= 1e-6f)
    {
        m_CurLookAtPos = m_TargetPos;
    }

    // 카메라가 LookEyePos를 넘어서까지 이동했다면 CurPos를 LookEyePos로 세팅해준다.
    Vec3 LeftMoveDir = (m_TargetPos - m_CurLookAtPos).Normalize();
    if (MoveDir.Dot(LeftMoveDir) <= 0.f)
    {
        m_CurLookAtPos = m_TargetPos;
    }
}

void CCameraController::UpdateLookDir()
{
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

            Quat PrevQuat = VectorToQuaternion(m_PrevLookDir);
            Quat LookQuat = VectorToQuaternion(m_LookDir);

            Quat SlerpQuat = Quat::Slerp(PrevQuat, LookQuat, t);
            m_CurLookDir = QuaternionToVector(SlerpQuat);
        }
    }
}

void CCameraController::UpdateLookDistance()
{
    // 이전 프레임의 거리가 목표하는 거리보다 크다면
    if (m_PrevDistance > m_LookDist)
    {
        m_CurDistance = m_PrevDistance - m_ZoomSpeed * DT;
    }

    // 이전 프레임의 거리가 목표하는 거리보다 작다면
    else if (m_PrevDistance < m_LookDist)
    {
        m_CurDistance = m_PrevDistance + m_ZoomSpeed * DT;
    }
}

void CCameraController::EditMode()
{
    // EditMode 토글
    if (KEY_TAP(KEY::TAB))
    {
        m_EditMode = m_EditMode ? false : true;
    }

    if (m_EditMode)
    {
        // 회전
        float RotSpeed = m_EditRotSpeed * XM_PI / 180.f * DT;

        // Right
        if (KEY_PRESSED(KEY::D))
        {
            Quaternion rotation = Quaternion::CreateFromAxisAngle(Vector3::Up, RotSpeed);
            m_LookDir = Vector3::Transform(m_LookDir, rotation);
            m_LookDir.Normalize();
        }

        // Left
        if (KEY_PRESSED(KEY::A))
        {
            Quaternion rotation = Quaternion::CreateFromAxisAngle(Vector3::Up, -RotSpeed);
            m_LookDir = Vector3::Transform(m_LookDir, rotation);
            m_LookDir.Normalize();
        }

        // Up
        if (KEY_PRESSED(KEY::W))
        {
            Quaternion rotation = Quaternion::CreateFromAxisAngle(Transform()->GetWorldDir(DIR_TYPE::RIGHT), RotSpeed);
            m_LookDir = Vector3::Transform(m_LookDir, rotation);
            m_LookDir.Normalize();
        }

        // Down
        if (KEY_PRESSED(KEY::S))
        {
            Quaternion rotation = Quaternion::CreateFromAxisAngle(Transform()->GetWorldDir(DIR_TYPE::RIGHT), -RotSpeed);
            m_LookDir = Vector3::Transform(m_LookDir, rotation);
            m_LookDir.Normalize();
        }

        // Distance
        // Zoom Out
        if (KEY_PRESSED(KEY::E))
        {
            m_LookDist += m_EditZoomSpeed * DT;
        }

        // Zoom In
        if (KEY_PRESSED(KEY::Q))
        {
            m_LookDist -= m_EditZoomSpeed * DT;
        }
    }
}

void CCameraController::Progress()
{    
    // Start와 End 사이에서 LookAtPos에 수직인 점을 찾기
    Vec3 ProgressRoad = m_ProgressEndPos - m_ProgressStartPos;
    Vec3 ToPrevLookAt = m_PrevLookAtPos - m_ProgressStartPos;

    float DotRes = ToPrevLookAt.Dot(ProgressRoad);
    float LengthSquared = ProgressRoad.Dot(ProgressRoad);

    // 아직 카메라가 바라봐야할 곳이 시작점 이전이거나, 끝점을 넘어갔을 경우 진행 X
    if (DotRes <= 0.f || DotRes > LengthSquared)
    {
        return;
    }

   float t = DotRes / LengthSquared;
   
   // t에 맞게 각 값들을 보간한다.
   m_ProgressStartDir.Normalize();
   m_ProgressEndDir.Normalize();

   Quat StartQuat = VectorToQuaternion(m_ProgressStartDir);
   Quat EndQuat = VectorToQuaternion(m_ProgressEndDir);
   Quat SlerpQuat = Quat::Slerp(StartQuat, EndQuat, t);
   m_LookDir = QuaternionToVector(SlerpQuat);
   m_LookDist = Lerp(m_ProgressStartDist, m_ProgressEndDist, t);
}

void CCameraController::ResetCamera()
{
    if (m_Target == nullptr)
        return;

    m_TargetPos = m_Target->Transform()->GetWorldPos();
    m_TargetPos += m_Offset;

    m_PrevLookDir = m_LookDir;
    m_CurLookDir = m_LookDir;
    m_PrevLookAtPos = m_TargetPos;
    m_CurLookAtPos = m_TargetPos;
    m_PrevDistance = m_LookDist;
    m_CurDistance = m_LookDist;

    Vec3 ResetPos = CalCamPos(m_TargetPos, m_LookDir, m_LookDist);

    m_LookEyePos = ResetPos;

    Transform()->SetWorldPos(ResetPos);
    Transform()->SetDirection(m_LookDir);

}

void CCameraController::ChangeMainTarget(CGameObject* _Target)
{
    m_Target = _Target;

    ResetCamera();
}

void CCameraController::ChangeMainTarget(wstring _TargetName)
{
    SetMainTarget(_TargetName);

    ResetCamera();
}

void CCameraController::ChangeLookSetting(Vec3 _LookDir, float _LookDist)
{
    m_LookDir = _LookDir;
    m_LookDist = _LookDist;
}

void CCameraController::ChangeFollwSpeedSetting(float _MinSpeed, float _MaxSpeed, float _Threshold)
{
    m_MinSpeed = _MinSpeed;
    m_MaxSpeed = _MaxSpeed;
    m_ThresholdDistance = _Threshold;
}

void CCameraController::ProgressSetup(Vec3 _StartPos, Vec3 _EndPos, Vec3 _StartDir, Vec3 _EndDir, float _StartDist, float _EndDist)
{   
    SetCameraSetup(CameraSetup::PROGRESS);

    m_ProgressStartPos = _StartPos;
    m_ProgressStartDir =_StartDir;
    m_ProgressStartDist = _StartDist;
    m_ProgressEndPos = _EndPos;
    m_ProgressEndDir =_EndDir;
    m_ProgressEndDist = _EndDist;
}

Vec3 CCameraController::CalCamPos(Vec3 _TargetWorldPos, Vec3 _LookDir, float _CamDist)
{
    _LookDir.Normalize();

    Vec3 RetVal = _TargetWorldPos - _LookDir * _CamDist;

    return RetVal;
}

UINT CCameraController::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_Offset, sizeof(Vec3), 1, _File);
    fwrite(&m_LookDir, sizeof(Vec3), 1, _File);
    fwrite(&m_LookDist, sizeof(float), 1, _File);

    fwrite(&m_MinSpeed, sizeof(float), 1, _File);
    fwrite(&m_MaxSpeed, sizeof(float), 1, _File);
    fwrite(&m_ThresholdDistance, sizeof(float), 1, _File);

    fwrite(&m_EditRotSpeed, sizeof(float), 1, _File);
    fwrite(&m_EditZoomSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CCameraController::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_Offset, sizeof(Vec3), 1, _File);
    fread(&m_LookDir, sizeof(Vec3), 1, _File);
    fread(&m_LookDist, sizeof(float), 1, _File);

    fread(&m_MinSpeed, sizeof(float), 1, _File);
    fread(&m_MaxSpeed, sizeof(float), 1, _File);
    fread(&m_ThresholdDistance, sizeof(float), 1, _File);

    fread(&m_EditRotSpeed, sizeof(float), 1, _File);
    fread(&m_EditZoomSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
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