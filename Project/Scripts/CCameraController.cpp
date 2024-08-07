#include "pch.h"
#include "CCameraController.h"

#include <Engine/CLevelMgr.h>

#include "CPlayerMgr.h"
#include "CKirbyMoveController.h"

CCameraController::CCameraController()
    : CScript(CAMERACONTROLLER)
    , m_Setup(CameraSetup::NORMAL)
    , m_Target(nullptr)
    , m_Offset(Vec3(0.f,50.f,0.f))
    , m_TargetPos(Vec3(0.f,0.f,0.f))
    , m_SubTarget(nullptr)
    , m_LookDir(0.f, -1.f, 1.f)
    , m_LookDist(200.f)
    , m_MinSpeed(10.f)
    , m_MaxSpeed(250.f)
    , m_ThresholdDistance(60.f)
    , m_RotationSpeed(50.f)
    , m_ZoomMinSpeed(0.f)
    , m_ZoomMaxSpeed(500.f)
    , m_ZoomThreshold(300.f)
    , m_EditRotSpeed(50.f)
    , m_EditZoomSpeed(500.f)
    , m_EditMode(false)
    , m_Weight(0.3f)
{

    AddScriptParam(SCRIPT_PARAM::VEC3, &m_Offset, "Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_SubTargetOffset, "SubTargetOffset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_LookDir, "Look Dir", 0.05f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_LookDist, "Look Distance", 1.f);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinSpeed, "MinSpeed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxSpeed, "MaxSpeed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ThresholdDistance, "Threshold Distance");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotationSpeed, "Rotation Speed");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ZoomMinSpeed, "Zoom Min Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ZoomMaxSpeed, "Zoom Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ZoomThreshold, "Zoom Threshold");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Weight, "Weight", 0.01f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_DistanceOffset, "DistanceOffset");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinDegreeX, "MinDirX");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxDegreeX, "MaxDirX");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinDegreeY, "MinDirY");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxDegreeY, "MaxDirY");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxBetweenTargetDist, "MaxBetweenTargetDist");



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

void CCameraController::SetSubTarget(wstring _TargetName)
{
    CLevel* CurrentLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    m_SubTarget = CurrentLevel->FindObjectByName(_TargetName);
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

    // Effect 초기화
    m_Effect[(UINT)EFFECT_TYPE::TILT_ANGLE] = {EFFECT_TYPE::TILT_ANGLE,false, 0.f, 0.f};
    m_Effect[(UINT)EFFECT_TYPE::SHAKE] = {EFFECT_TYPE::SHAKE,false, 0.f, 0.f};

    // 카메라 세팅
    SaveSetting();
    SaveInitSetting();

    m_Setup = CameraSetup::NORMAL;

    m_TargetPos = m_Target->Transform()->GetWorldPos();
    m_LookAtPos = m_TargetPos + m_Offset;

    m_PrevLookDir = m_LookDir;
    m_CurLookDir = m_LookDir;
    m_PrevLookAtPos = m_LookAtPos;
    m_CurLookAtPos = m_LookAtPos;
    m_PrevDistance = m_LookDist;
    m_CurDistance = m_LookDist;

    Vec3 InitPos = CalCamPos(m_LookAtPos, m_LookDir, m_LookDist);
    
    m_LookEyePos = InitPos;

    Transform()->SetWorldPos(InitPos);
    Transform()->SetDirection(m_LookDir);
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
    ApplyTargetOffset();

    // 현재 Setup에 맞게 카메라의 LookDir, LookDist, LookAtPos를 수정한다.
    SetUpProc();

    // LookAtPos에 대한Offset 적용
    ApplyOffset();

    // LookAtPos, LookDir, LookDist에 맞게 현재 프레임의 위치, 각도, 거리를 업데이트 한다.
    UpdateLookAtPos();
    UpdateLookDir();
    UpdateLookDistance();

    m_LookDirQuat = VectorToQuaternion(m_CurLookDir);

    // Effect 처리
    ProcessEffet();


    // ========================= Eye Pos Update =========================
    // ==================== Camera Transform Update ====================
    Transform()->SetWorldRotation(m_LookDirQuat);
    m_LookEyePos = CalCamPos(m_CurLookAtPos, m_CurLookDir, m_CurDistance);
    Transform()->SetWorldPos(m_LookEyePos);
    //Transform()->SetDirection(m_CurLookDir);


    // @DEBUG
    //GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), 200.f, Vec3(1.f, 0.f, 0.f), true);
    //GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::RIGHT), 200.f, Vec3(0.f, 1.f, 0.f), true);
    //GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::UP), 200.f, Vec3(0.f, 0.f, 1.f), true);
    //GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), m_CurLookDir, m_LookDist, Vec3(0.f, 0.f, 0.f), true);
}

void CCameraController::SetUpProc()
{
    switch (m_Setup)
    {
    case CameraSetup::NORMAL:
        Normal();
        break;
    case CameraSetup::PROGRESS:
        Progress();
        break;
    case CameraSetup::TWOTARGET:
        TwoTarget();
        break;
    case CameraSetup::BOSSSETUP:
        Boss();
        break;
    case CameraSetup::FIXEDVIEW:
        FixedView();
    default:
        break;
    }
}

void CCameraController::UpdateTargetPos()
{
    // 타겟의 현재 위치 업데이트
    if (nullptr != m_Target)
    {
        if (m_Target->GetName() == L"Main Player")
        {
            // 커비일 경우에는 점프할 때 따라가지 않음
            Vec3 TargetPos = m_Target->Transform()->GetWorldPos();

            m_TargetPos.x = TargetPos.x;
            m_TargetPos.z = TargetPos.z;

            RaycastHit Hit = PLAYERCTRL->GetRay();

            if (PLAYERCTRL->IsGround())
            {
                m_TargetPos.y = TargetPos.y;
            }
            else
            {
                if (Hit.pCollisionObj != nullptr && Hit.Distance < 100.f)
                {
                    m_TargetPos.y = Hit.Point.y;
                }
                else
                {
                    m_TargetPos.y = TargetPos.y;
                }
            }
        }
        else
        {
            m_TargetPos = m_Target->Transform()->GetWorldPos();
        }
    }

    if (nullptr != m_SubTarget)
    {
        m_SubTargetPos = m_SubTarget->Transform()->GetWorldPos();
    }

}

void CCameraController::ApplyTargetOffset()
{
    // 각 타겟에 대한 Offset을 적용한다.
    if (nullptr != m_Target)
    {
        m_TargetPos += m_TargetOffset;
    }

    if (nullptr != m_SubTarget)
    {
        m_SubTargetPos += m_SubTargetOffset;
    }
}

void CCameraController::ApplyOffset()
{
    m_LookAtPos += m_Offset;
}

void CCameraController::UpdateLookAtPos()
{
    // =========================== Pos Update ===========================
    Vec3 Diff = m_LookAtPos - m_PrevLookAtPos;
    float MoveLength = Diff.Length();
    Vec3 MoveDir = Diff.Normalize();

    float Ratio = clamp((MoveLength / m_ThresholdDistance), 0.f, 1.f) * XM_PI / 2.f;
    float CamSpeed = m_MinSpeed + (m_MaxSpeed - m_MinSpeed) * sinf(Ratio);

    m_CurLookAtPos = m_PrevLookAtPos + MoveDir * CamSpeed * DT_ENGINE;

    // =========================== 예외 처리 =============================
    // 보간 값이 목표값과 비슷하다면 그대로 세팅해준다.
    if ((m_LookAtPos - m_CurLookAtPos).Length() <= 1e-6f)
    {
        m_CurLookAtPos = m_LookAtPos;
    }

    // 카메라가 LookEyePos를 넘어서까지 이동했다면 CurPos를 LookEyePos로 세팅해준다.
    Vec3 LeftMoveDir = (m_LookAtPos - m_CurLookAtPos).Normalize();
    if (MoveDir.Dot(LeftMoveDir) <= 0.f)
    {
        m_CurLookAtPos = m_LookAtPos;
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
            
            float MaxRotationStep = m_RotationSpeed * XM_PI / 180.f * DT_ENGINE;
            //float t = min(MaxRotationStep / degrees, 1.0f); // 등속 운동

            Quat PrevQuat = VectorToQuaternion(m_PrevLookDir);
            Quat LookQuat = VectorToQuaternion(m_LookDir);

            Quat SlerpQuat = Quat::Slerp(PrevQuat, LookQuat, MaxRotationStep);
            m_CurLookDir = QuaternionToVector(SlerpQuat);
        }
    }
}

void CCameraController::UpdateLookDistance()
{
    // 현재 프레임에서의 ZoomSpeed 구하기
    float DiffDist = fabs(m_LookDist - m_PrevDistance);
    float Ratio = clamp((DiffDist / m_ZoomThreshold), 0.f, 1.f) * XM_PI * 0.5f;
    float ZoomSpeed = m_ZoomMinSpeed + (m_ZoomMaxSpeed - m_ZoomMinSpeed) * sinf(Ratio);


    // Zoom In인 경우 ZoomSpeed를 반대로 한다.
    if (m_PrevDistance > m_LookDist)
        ZoomSpeed *= -1.f;

    // Dist 조절
    m_CurDistance = m_PrevDistance + ZoomSpeed * DT_ENGINE;

    // 예외처리
    // 보정해야하는 거리가 너무 가깝다면 CurDist를 고정
    if (fabs(m_LookDist - m_CurDistance) < 0.1f)
    {
        m_CurDistance = m_LookDist;
    }

    // Dist가 원했던 경우보다 더 멀리간 경우 CurDist를 고정
    if (ZoomSpeed * (m_CurDistance - m_LookDist) > 0.f)
    {
        m_CurDistance = m_LookDist;
    }
}

void CCameraController::SaveSetting()
{
    m_SaveSetting.LookDir = m_LookDir;
    m_SaveSetting.LookDist = m_LookDist;
    m_SaveSetting.MaxSpeed = m_MaxSpeed;
    m_SaveSetting.MinSpeed = m_MinSpeed;
    m_SaveSetting.Offset = m_Offset;
    m_SaveSetting.RotationSpeed = m_RotationSpeed;
    m_SaveSetting.ThresholdDistance = m_ThresholdDistance;
    m_SaveSetting.ZoomMaxSpeed = m_ZoomMaxSpeed;
    m_SaveSetting.ZoomMinSpeed = m_ZoomMinSpeed;
    m_SaveSetting.ZoomThreshold = m_ZoomThreshold;
}

void CCameraController::SaveInitSetting()
{
    m_InitSetting.LookDir = m_LookDir;
    m_InitSetting.LookDist = m_LookDist;
    m_InitSetting.MaxSpeed = m_MaxSpeed;
    m_InitSetting.MinSpeed = m_MinSpeed;
    m_InitSetting.Offset = m_Offset;
    m_InitSetting.RotationSpeed = m_RotationSpeed;
    m_InitSetting.ThresholdDistance = m_ThresholdDistance;
    m_InitSetting.ZoomMaxSpeed = m_ZoomMaxSpeed;
    m_InitSetting.ZoomMinSpeed = m_ZoomMinSpeed;
    m_InitSetting.ZoomThreshold = m_ZoomThreshold;
}

void CCameraController::LoadSetting()
{
    m_LookDir = m_SaveSetting.LookDir;
    m_LookDist = m_SaveSetting.LookDist;
    m_MaxSpeed = m_SaveSetting.MaxSpeed;
    m_MinSpeed = m_SaveSetting.MinSpeed;
    m_Offset = m_SaveSetting.Offset;
    m_RotationSpeed = m_SaveSetting.RotationSpeed;
    m_ThresholdDistance = m_SaveSetting.ThresholdDistance;
    m_ZoomMaxSpeed = m_SaveSetting.ZoomMaxSpeed;
    m_ZoomMinSpeed = m_SaveSetting.ZoomMinSpeed;
    m_ZoomThreshold = m_SaveSetting.ZoomThreshold;  
}

void CCameraController::LoadInitSetting()
{
    m_LookDir = m_InitSetting.LookDir;
    m_LookDist = m_InitSetting.LookDist;
    m_MaxSpeed = m_InitSetting.MaxSpeed;
    m_MinSpeed = m_InitSetting.MinSpeed;
    m_Offset = m_InitSetting.Offset;
    m_RotationSpeed = m_InitSetting.RotationSpeed;
    m_ThresholdDistance = m_InitSetting.ThresholdDistance;
    m_ZoomMaxSpeed = m_InitSetting.ZoomMaxSpeed;
    m_ZoomMinSpeed = m_InitSetting.ZoomMinSpeed;
    m_ZoomThreshold = m_InitSetting.ZoomThreshold;  
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
        float RotSpeed = m_EditRotSpeed * XM_PI / 180.f * DT_ENGINE;

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

        // Down
        if (KEY_PRESSED(KEY::S))
        {
            Quaternion rotation = Quaternion::CreateFromAxisAngle(Transform()->GetWorldDir(DIR_TYPE::RIGHT), RotSpeed);
            m_LookDir = Vector3::Transform(m_LookDir, rotation);
            m_LookDir.Normalize();
        }

        // Up
        if (KEY_PRESSED(KEY::W))
        {
            Quaternion rotation = Quaternion::CreateFromAxisAngle(Transform()->GetWorldDir(DIR_TYPE::RIGHT), -RotSpeed);
            m_LookDir = Vector3::Transform(m_LookDir, rotation);
            m_LookDir.Normalize();
        }

        // Distance
        // Zoom Out
        if (KEY_PRESSED(KEY::E))
        {
            m_LookDist += m_EditZoomSpeed * DT_ENGINE;
        }

        // Zoom In
        if (KEY_PRESSED(KEY::Q))
        {
            m_LookDist -= m_EditZoomSpeed * DT_ENGINE;
        }
    }
}

void CCameraController::Normal()
{
    m_LookAtPos = m_TargetPos;
}

void CCameraController::Progress()
{    
    m_LookAtPos = m_TargetPos;

    // Start와 End 사이에서 LookAtPos에 수직인 점을 찾기
    Vec3 ProgressRoad = m_ProgressEndPos - m_ProgressStartPos;
    Vec3 ToPrevLookAt = m_LookAtPos - m_ProgressStartPos;

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
   m_Offset = Vector3::Lerp(m_ProgressStartOffset, m_ProgressEndOffset, t);
   m_LookDir = QuaternionToVector(SlerpQuat);
   m_LookDist = Lerp(m_ProgressStartDist, m_ProgressEndDist, t);

   
}

void CCameraController::TwoTarget()
{
    if (m_Target == nullptr || m_SubTarget == nullptr)
        return;

    // 두 물체의 중심점 계산
    Vec3 Center = (m_TargetPos + m_SubTargetPos) * 0.5f;

    // 두 물체의 상대 위치 벡터 계산
    Vec3 ToTarget1 = m_TargetPos - Center;
    Vec3 ToTarget2 = m_SubTargetPos - Center;

    // Camera의 Front, UP, Right 를 가져온다.
    Vec3 LookDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 RightDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
    Vec3 UpDir = Transform()->GetWorldDir(DIR_TYPE::UP);

    // 두 물체 사이의 가로 및 세로 거리 계산
    float HorizontalDistance = fabs(ToTarget1.Dot(RightDir)) + fabs(ToTarget2.Dot(RightDir));
    float VerticalDistance = fabs(ToTarget1.Dot(UpDir)) + fabs(ToTarget2.Dot(UpDir));
    float DepthDistance = fabs(ToTarget1.Dot(LookDir)) + fabs(ToTarget2.Dot(LookDir));

    // FOV와 Aspect Ratio를 기반으로 Distance Scale Factor 계산
    float AspectRatio = Camera()->GetAspectRatio();
    float VerticalFOV = Camera()->GetFOV();
    float HorizontalFOV = 2 * atan(tan(VerticalFOV / 2) * AspectRatio);

    // 필요한 최소 거리 계산
    float RequiredHorizontalDistance = (HorizontalDistance / 2.0f) / tan(HorizontalFOV / 2.0f);
    float RequiredVerticalDistance = (VerticalDistance / 2.0f) / tan(VerticalFOV / 2.0f);

    // 카메라의 거리 중 더 큰 값을 선택
    float RequiredDistance = max(RequiredHorizontalDistance, RequiredVerticalDistance);

    m_LookDist = RequiredDistance + DepthDistance / 2.f + m_DistanceOffset; 

    if (m_LookDist < m_MinDist)
    {
        m_LookDist = m_MinDist;
    }

    m_LookAtPos = Center;
}


void CCameraController::Boss()
{
    if (m_Target == nullptr || m_SubTarget == nullptr)
        return;

    // 가중치를 적용한 중심점 계산
    Vec3 Center = m_TargetPos * (1.0f - m_Weight) + m_SubTargetPos * m_Weight;

    // 두 물체의 상대 위치 벡터 계산
    Vec3 ToTarget1 = m_TargetPos - Center;
    Vec3 ToTarget2 = m_SubTargetPos - Center;

    // Camera의 Front, Right, Up 벡터를 구한다.
    Vec3 LookDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 RightDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
    Vec3 UpDir = Transform()->GetWorldDir(DIR_TYPE::UP);

    //Vec3 LookDir = m_LookDir;
    //Vec3 RightDir = Vec3(0.f, 1.f, 0.f).Cross(LookDir).Normalize();
    //Vec3 UpDir = LookDir.Cross(RightDir).Normalize();

    // 두 물체 사이의 가로, 세로, 깊이 거리 계산
    float HorizontalDistanceToTarget1 = fabs(ToTarget1.Dot(RightDir));
    float VerticalDistanceToTarget1 = fabs(ToTarget1.Dot(UpDir));
    float DepthDistanceToTarget1 = -ToTarget1.Dot(LookDir);

    float HorizontalDistanceToTarget2 = fabs(ToTarget2.Dot(RightDir));
    float VerticalDistanceToTarget2 = fabs(ToTarget2.Dot(UpDir));
    float DepthDistanceToTarget2 = -ToTarget2.Dot(LookDir);

    // FOV와 Aspect Ratio를 기반으로 Distance Scale Factor 계산
    float AspectRatio = Camera()->GetAspectRatio();
    float VerticalFOV = Camera()->GetFOV();
    float HorizontalFOV = 2 * atan(tan(VerticalFOV / 2) * AspectRatio);

    // 필요한 최소 거리 계산
    float RequiredHorizontalDistanceToTarget1 = (HorizontalDistanceToTarget1) / tan(HorizontalFOV / 2.f);
    float RequiredHorizontalDistanceToTarget2 = (HorizontalDistanceToTarget2) / tan(HorizontalFOV / 2.f);
    float RequiredVerticalDistanceToTarget1 = (VerticalDistanceToTarget1) / tan(VerticalFOV / 2.f);
    float RequiredVerticalDistanceToTarget2 = (VerticalDistanceToTarget2) / tan(VerticalFOV / 2.f);

    // 각 타겟이 보이기위해 필요한 최소거리
    float RequiredDistanceTarget1 = RequiredHorizontalDistanceToTarget1 > RequiredVerticalDistanceToTarget1 ? RequiredHorizontalDistanceToTarget1
                                                                                                            : RequiredVerticalDistanceToTarget1;
    float RequiredDistanceTarget2 = RequiredHorizontalDistanceToTarget2 > RequiredVerticalDistanceToTarget2 ? RequiredHorizontalDistanceToTarget2
                                                                                                            : RequiredVerticalDistanceToTarget2;

    // 깊이에 대한 거리를 각각 더해준다.
    RequiredDistanceTarget1 += DepthDistanceToTarget1;
    RequiredDistanceTarget2 += DepthDistanceToTarget2;

    // 필요한 거리가 더 먼경우를 현재의 거리로 정해준다.
    float RequiredDistance = RequiredDistanceTarget1 > RequiredDistanceTarget2 ? RequiredDistanceTarget1 : RequiredDistanceTarget2;

    // ================== Direction =================

    // 수평 각도 계산
    Vec3 Dir = m_SubTargetPos - m_TargetPos;
    Dir.y = 0.f;
    Dir.Normalize();

    float CurBetweenTargetDist = (m_SubTargetPos - m_TargetPos).Length();

    // 타겟과의 거리에 따라 x축으로 카메라를 회전시킨다.
    float Ratio = clamp((CurBetweenTargetDist / m_MaxBetweenTargetDist), 0.f, 1.f) * XM_PI * 0.5f;
    float Degree = m_MinDegreeX + (m_MaxDegreeX - m_MinDegreeX) * sinf(Ratio);

    if (Degree != 0.f)
    {
        Quaternion rotation = Quaternion::CreateFromAxisAngle(RightDir, Degree * XM_PI / 180.f);
        Dir = Vector3::Transform(Dir, rotation);
    }

    // 타겟과의 거리에 따라 Y축으로 카메라를 회전시킨다.
    Ratio = clamp((CurBetweenTargetDist / m_MaxBetweenTargetDist), 0.f, 1.f) * XM_PI * 0.5f;
    Degree = m_MinDegreeY + (m_MaxDegreeY - m_MinDegreeY) * sinf(Ratio);

    if (Degree != 0.f)
    {
        Quaternion rotation = Quaternion::CreateFromAxisAngle(UpDir, Degree * XM_PI / 180.f);
        Dir = Vector3::Transform(Dir, rotation);
    }


    Dir.Normalize();
    m_LookDir = Dir;

    m_LookDist = RequiredDistance + m_DistanceOffset;
    m_LookAtPos = Center;
}

void CCameraController::FixedView()
{
    // 카메라의 위치는 고정되고 
    Vec3 ToTargetFronCamera = m_TargetPos - m_FixedViewPos;

    m_LookAtPos = m_TargetPos;
    m_LookDist = ToTargetFronCamera.Length();
    m_LookDir = ToTargetFronCamera.Normalize();
}

float simpleNoise(float t)
{
    return sin(t) * (rand() / float(RAND_MAX));
}


void CCameraController::ProcessEffet()
{
    for (int i = 0; i < (UINT)EFFECT_TYPE::END; ++i)
    {
        CamEffect& CurEffet = m_Effect[i];

        if (!CurEffet.Running)
            continue;

        CurEffet.Acc += DT_ENGINE;

        switch (CurEffet.EffetType)
        {
        case EFFECT_TYPE::TILT_ANGLE: 
        {
            Transform()->SetDirection(m_CurLookDir);

            float tiltDuration = 0.5f;        // 초기 회전 시간
            float oscillationDuration = 3.0f; // 흔들림 시간
            float returnDuration = 0.5f;      // 복귀 시간

            float totalDuration = tiltDuration + oscillationDuration + returnDuration;

            if (CurEffet.Acc < tiltDuration)
            {
                // 초기 회전 (90도)
                float Alpha = CurEffet.Acc / tiltDuration;

                m_LookDirQuat = m_LookDirQuat * Quaternion::CreateFromAxisAngle(Transform()->GetLocalDir(DIR_TYPE::FRONT).Normalize(),
                                                                                Alpha * XMConvertToRadians(-90.0f));
            }
            else if (CurEffet.Acc < tiltDuration + oscillationDuration)
            {

                // 흔들림
                float minAngle = XMConvertToRadians(-80.0f);
                float maxAngle = XMConvertToRadians(-100.0f);
                float centerAngle = XMConvertToRadians(-90.0f);
                float frequency = 1.0f;

                float t = (CurEffet.Acc - tiltDuration) / frequency;

                // 사인파를 사용하여 각도 계산
                float oscillationAngle = minAngle + (maxAngle - minAngle) * 0.5f * (1.0f + sinf(2.0f * XM_PI * t));

                // 카메라를 회전시키는 쿼터니언 생성
                m_LookDirQuat =
                    m_LookDirQuat * Quaternion::CreateFromAxisAngle(Transform()->GetLocalDir(DIR_TYPE::FRONT).Normalize(), oscillationAngle);
            }
            else if (CurEffet.Acc < totalDuration)
            {
                // 원래 상태로 복귀
                float alpha = (CurEffet.Acc - tiltDuration - oscillationDuration) / returnDuration;
                m_LookDirQuat = m_LookDirQuat * Quaternion::CreateFromAxisAngle(Transform()->GetLocalDir(DIR_TYPE::FRONT).Normalize(),
                                                                                (1.0f - alpha) * XMConvertToRadians(-90.0f));
            }

            Transform()->SetWorldRotation(m_LookDirQuat);
            /*m_LookDir = QuaternionToVector(m_LookDirQuat);*/
            //m_LookEyePos = CalCamPos(m_CurLookAtPos, Transform()->GetLocalDir(DIR_TYPE::FRONT), m_CurDistance);
        }
        break;

        case EFFECT_TYPE::SHAKE: 
        {
            m_LookDirQuat = m_LookDirQuat *
                            Quaternion::CreateFromAxisAngle(Transform()->GetWorldDir(DIR_TYPE::RIGHT),
                                                XMConvertToRadians(simpleNoise(CurEffet.Acc * m_ShakeFrequency.x) * m_ShakeIntencity * DT_ENGINE)) *
                            Quaternion::CreateFromAxisAngle(Transform()->GetWorldDir(DIR_TYPE::UP),
                                                XMConvertToRadians(simpleNoise(CurEffet.Acc * m_ShakeFrequency.y) * m_ShakeIntencity * DT_ENGINE)) *
                            Quaternion::CreateFromAxisAngle(Transform()->GetWorldDir(DIR_TYPE::FRONT),
                                                XMConvertToRadians(simpleNoise(CurEffet.Acc * m_ShakeFrequency.z) * m_ShakeIntencity * DT_ENGINE));
        }
            break;

        default:
            break;
        }

        if (CurEffet.Acc > CurEffet.Duration)
        {
            CurEffet.Running = false;
            CurEffet.Acc = 0.f;
            CurEffet.Duration = 0.f;
        }
    }

}


void CCameraController::ResetCamera()
{
    if (m_Target == nullptr)
        return;

    UpdateTargetPos();
    SetUpProc();
    ApplyOffset();

    m_PrevLookDir = m_LookDir;
    m_CurLookDir = m_LookDir;
    m_PrevLookAtPos = m_LookAtPos;
    m_CurLookAtPos = m_LookAtPos;
    m_PrevDistance = m_LookDist;
    m_CurDistance = m_LookDist;

    Vec3 ResetPos = CalCamPos(m_LookAtPos, m_LookDir, m_LookDist);

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

void CCameraController::Normal(bool _IsImmediate)
{
   SetCameraSetup(CameraSetup::NORMAL);

   if (_IsImmediate)
   {
       ResetCamera();
   }
}

void CCameraController::ProgressSetup(Vec3 _StartPos, Vec3 _EndPos, Vec3 _StartOffset, Vec3 _EndOffset, Vec3 _StartDir, Vec3 _EndDir,
                                      float _StartDist, float _EndDist)
{   
    SetCameraSetup(CameraSetup::PROGRESS);

    m_ProgressStartOffset = _StartOffset;
    m_ProgressEndOffset = _EndOffset;

    m_ProgressStartPos = _StartPos;
    m_ProgressStartDir =_StartDir;
    m_ProgressStartDist = _StartDist;
    m_ProgressEndPos = _EndPos;
    m_ProgressEndDir =_EndDir;
    m_ProgressEndDist = _EndDist;
}

void CCameraController::TwoTarget(CGameObject* _SubTarget, bool _bChangeLookDir, Vec3 _LookDir, float _DistanceOffset, float _MinDist)
{
    if (_SubTarget == nullptr)
        return;

    m_Setup = CameraSetup::TWOTARGET;

    m_SubTarget = _SubTarget;
    m_SubTargetPos = m_SubTarget->Transform()->GetWorldPos();
    if (_bChangeLookDir)
        m_LookDir = _LookDir.Normalize();
    m_DistanceOffset = _DistanceOffset;
    m_MinDist = _MinDist;

    m_Offset = Vec3(0.f, 0.f, 0.f);
    m_TargetOffset = Vec3(0.f, 0.f, 0.f);
    m_SubTargetOffset = Vec3(0.f, 0.f, 0.f);
}

void CCameraController::TwoTarget(wstring _SubTargetName, Vec3 _LookDir, float _DistanceOffset, float _MinDist)
{
    m_Setup = CameraSetup::TWOTARGET;

    m_SubTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(_SubTargetName);

    // 해당 이름을 가진 타겟이 없다면
    if (m_SubTarget == nullptr)
    {
        m_Setup = CameraSetup::NORMAL;
        return;
    }

    m_SubTargetPos = m_SubTarget->Transform()->GetWorldPos();
    m_LookDir = _LookDir.Normalize();
    m_DistanceOffset = _DistanceOffset;
    m_MinDist = _MinDist;

    m_Offset = Vec3(0.f, 0.f, 0.f);
    m_TargetOffset = Vec3(0.f, 0.f, 0.f);
    m_SubTargetOffset = Vec3(0.f, 0.f, 0.f);
}

void CCameraController::Boss(CGameObject* _SubTarget, float _DistanceOffset, float _MinDegree, float _MaxDegree,
                                     float _m_MaxBetweenTargetDist, float _Weight)
{
    if (_SubTarget == nullptr)
        return;

    m_Setup = CameraSetup::BOSSSETUP;

    m_SubTarget = _SubTarget;
    m_SubTargetPos = m_SubTarget->Transform()->GetWorldPos();
    m_DistanceOffset = _DistanceOffset;

    m_Offset = Vec3(0.f, 0.f, 0.f);
    m_TargetOffset = Vec3(0.f, 0.f, 0.f);
    m_SubTargetOffset = Vec3(0.f, 0.f, 0.f);
}

void CCameraController::Boss(wstring _SubTargetName, float _DistanceOffset, float _MinDegree, float _MaxDegree, float _m_MaxBetweenTargetDist,
                                     float _Weight)
{
    m_Setup = CameraSetup::BOSSSETUP;

    m_SubTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(_SubTargetName);

    // 해당 이름을 가진 타겟이 없다면
    if (m_SubTarget == nullptr)
    {
        m_Setup = CameraSetup::NORMAL;
        return;
    }

    m_SubTargetPos = m_SubTarget->Transform()->GetWorldPos();
    m_DistanceOffset = _DistanceOffset;

    m_Offset = Vec3(0.f, 0.f, 0.f);
    m_TargetOffset = Vec3(0.f, 0.f, 0.f);
    m_SubTargetOffset = Vec3(0.f, 80.f, 0.f);
    m_SubTargetOffset = Vec3(0.f, 0.f, 0.f);
}

void CCameraController::FixedView(bool _IsImmediate, Vec3 _FixedViewPos)
{
    SetCameraSetup(CameraSetup::FIXEDVIEW);

    if (_FixedViewPos == Vec3(0.f, 0.f, 0.f))
    {
        m_FixedViewPos = m_LookEyePos;
    }
    else
    {
        m_FixedViewPos = _FixedViewPos;
    }

    if (_IsImmediate)
    {
        ResetCamera();
    }
}

void CCameraController::Shake(float _Duration, float _Frequency, float _Intencity)
{
    CamEffect& ShakeEffet = m_Effect[(UINT)EFFECT_TYPE::SHAKE];

    if (ShakeEffet.Running && _Duration < (ShakeEffet.Duration - ShakeEffet.Acc))
        return;

    ShakeEffet.Running = true;
    ShakeEffet.Duration = _Duration;
    ShakeEffet.Acc = 0.f;

    m_ShakeIntencity = _Intencity;
    m_ShakeFrequency = {GetRandomfloat(10.f, _Frequency), GetRandomfloat(10.f, _Frequency), GetRandomfloat(10.f, _Frequency)};
}

void CCameraController::Tilt(float _Duration, float _Frequency)
{
    CamEffect& TiltEffet = m_Effect[(UINT)EFFECT_TYPE::TILT_ANGLE];

    if (TiltEffet.Running && _Duration < (TiltEffet.Duration - TiltEffet.Acc))
        return;

    TiltEffet.Running = true;
    TiltEffet.Duration = _Duration;
    TiltEffet.Acc = 0.f;

    m_TiltFrequency = _Frequency;
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

    fwrite(&m_RotationSpeed, sizeof(float), 1, _File);

    fwrite(&m_ZoomMinSpeed, sizeof(float), 1, _File);
    fwrite(&m_ZoomMaxSpeed, sizeof(float), 1, _File);
    fwrite(&m_ZoomThreshold, sizeof(float), 1, _File);

    fwrite(&m_EditRotSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
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

    fread(&m_RotationSpeed, sizeof(float), 1, _File);

    fread(&m_ZoomMinSpeed, sizeof(float), 1, _File);
    fread(&m_ZoomMaxSpeed, sizeof(float), 1, _File);
    fread(&m_ZoomThreshold, sizeof(float), 1, _File);

    fread(&m_EditRotSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
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