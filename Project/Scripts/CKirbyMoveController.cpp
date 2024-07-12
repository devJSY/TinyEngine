#include "pch.h"
#include "CKirbyMoveController.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CKirbyUnitScript.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CPhysicsMgr.h>

CKirbyMoveController::CKirbyMoveController()
    : CScript(KIRBYMOVECONTROLLER)
    , m_Input{0.f, 0.f, 0.f}
    , m_CurDir{}
    , m_TowardDir{}
    , m_MoveDir{0.f, 0.f, 0.f}
    , m_GroundNormal{0.f, 1.f, 0.f}
    , m_ForceDirInfos{}
    , m_MoveVelocity{}
    , m_Speed(10.f)
    , m_MaxSpeed(15.f)
    , m_RotSpeed(50.f)
    , m_JumpPower(10.f)
    , m_Gravity(-20.f)
    , m_bMoveLock(false)
    , m_bDirLock(false)
    , m_bJumpLock(false)
    , m_bJump(false)
    , m_bActiveFriction(false)
    , m_HoveringLimitHeight(500.f)
    , m_HoveringHeight(0.f)
    , m_AddVelocity{0.f,0.f,0.f}
    , m_Friction(0.f)
    , m_HoveringMinSpeed(-5.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotSpeed, "Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Gravity, "Gravity");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HoveringLimitHeight, "HoveringLimit");
}

CKirbyMoveController::CKirbyMoveController(const CKirbyMoveController& _Origin)
    : CScript(_Origin)
    , m_CurDir{}
    , m_TowardDir{}
    , m_Input{0.f, 0.f, 0.f}
    , m_ForceDirInfos{}
    , m_MoveDir{0.f, 0.f, 0.f}
    , m_GroundNormal{0.f, 1.f, 0.f}
    , m_MoveVelocity{}
    , m_Speed(_Origin.m_Speed)
    , m_MaxSpeed(_Origin.m_MaxSpeed)
    , m_RotSpeed(_Origin.m_RotSpeed)
    , m_JumpPower(_Origin.m_JumpPower)
    , m_Gravity(_Origin.m_Gravity)
    , m_bMoveLock(false)
    , m_bDirLock(false)
    , m_bJumpLock(false)
    , m_bJump(false)
    , m_bActiveFriction(false)
    , m_HoveringLimitHeight(_Origin.m_HoveringLimitHeight)
    , m_HoveringMinSpeed(_Origin.m_HoveringMinSpeed)
    , m_AddVelocity{0.f, 0.f, 0.f}
    , m_Friction(_Origin.m_Friction)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotSpeed, "Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Gravity, "Gravity");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HoveringLimitHeight, "HoveringLimit");
}

CKirbyMoveController::~CKirbyMoveController()
{
}

void CKirbyMoveController::begin()
{
    // 플레이어는 캐릭터 컨트롤러 컴포넌트를 무조건 소유하도록 추가
    if (nullptr == CharacterController())
    {
        GetOwner()->AddComponent(new CCharacterController);
    }

    m_CurDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    m_TowardDir = m_CurDir;

    //@TODO UNIT쪽에서 받아오기
    //m_Speed = PLAYERUNIT->GetCurInfo().Speed;
    //m_JumpPower = PLAYERUNIT->GetCurInfo().JumpPower;
    m_Speed = 10.f;
    m_JumpPower = 10.f;
    m_RotSpeed = 50.f;
    m_Gravity = -20.f;
}

void CKirbyMoveController::tick()
{
    // Key 입력 확인
    Input();

    // 캐릭터의 수평방향 정하기
    SetDir();

    // 이동
    Move();

    // 표면 정렬 (보류)
    // SurfaceAlignment();

    // Debug : 방향 표시
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), 30.f, Vec3(0.f, 1.f, 0.f), true);
    // GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::RIGHT), 30.f, Vec3(1.f, 0.f, 0.f), true);
    // GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::UP), 30.f, Vec3(0.f, 0.f, 1.f), true);
}

void CKirbyMoveController::Input()
{
    // 키 입력 정보
    m_Input = {0.f, 0.f, 0.f};
    // 움직임 방향 정보(World좌표계)
    m_MoveDir = {0.f, 0.f, 0.f};
    // 커비 방향 정보
    m_CurDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

    // 입력 받기
    // ↑ ↓
    if (KEY_TAP(KEY::UP) || KEY_PRESSED(KEY::UP))
    {
        m_Input += Vec3(0.f, 0.f, 1.f);
    }
    if (KEY_TAP(KEY::DOWN) || KEY_PRESSED(KEY::DOWN))
    {
        m_Input += Vec3(0.f, 0.f, -1.f);
    }

    // ← →
    if (KEY_TAP(KEY::RIGHT) || KEY_PRESSED(KEY::RIGHT))
    {
        m_Input += Vec3(1.f, 0.f, 0.f);
    }
    if (KEY_TAP(KEY::LEFT) || KEY_PRESSED(KEY::LEFT))
    {
        m_Input += Vec3(-1.f, 0.f, 0.f);
    }

    // m_MoveDir 계산
    // Main Camera 가져오기
    CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();

    if (nullptr == pCam)
        return;

    Vec3 Front = pCam->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 Right = pCam->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

    // y축 이동은 컴포넌트에 의해서 처리
    Front.y = 0.f;
    Right.y = 0.f;

    if (!m_bMoveLock)
    {
        m_MoveDir = XMVectorAdd(XMVectorScale(Front, m_Input.z), XMVectorScale(Right, m_Input.x));
        m_MoveDir.Normalize();
    }
}

void CKirbyMoveController::SetDir()
{
    // 강제 방향 변경이 있다면 해당 방향을 반영
    if (!m_ForceDirInfos.empty())
    {
        UINT Priority = (UINT)ForceDirType::END;
        Vec3 ForceDir = m_TowardDir;

        for (size_t i = 0; i < m_ForceDirInfos.size(); ++i)
        {
            if ((UINT)m_ForceDirInfos[i].Type < Priority)
            {

                Priority = (UINT)m_ForceDirInfos[i].Type;
                ForceDir = m_ForceDirInfos[i].Dir;
            }
        }

        ForceDir.y = 0.f;
        ForceDir.Normalize();

        // 가장 우선순위가 높은 방향을 적용
        Transform()->SetDirection(ForceDir);
        m_TowardDir = ForceDir;

        m_ForceDirInfos.clear();
        return;
    }

    // 키 입력시 바라봐야할 방향을 수정
    if (m_Input.Length() > 0.f && m_bDirLock == false)
    {
        m_TowardDir = m_MoveDir;
    }

    // 방향 보간중 Lock 걸릴 경우 바라봐야할 방향을 현재 방향으로 고정
    if (m_bDirLock)
    {
        m_TowardDir = m_CurDir;
    }

    // 바라봐야할 방향과 현재 방향이 정반대일 경우 예외처리
    if (m_CurDir.Dot(m_TowardDir) - (m_CurDir.Length() * m_TowardDir.Length() * -1.f) < 0.0001f)
    {
        Vec3 Rot = Transform()->GetRelativeRotation();
        Rot.y += XM_PI / 180.f;
        Transform()->SetRelativeRotation(Rot);
    }

    // 구면보간을 이용해서 물체의 새로운 방향을 정의
    Transform()->SetDirection(Vector3::SmoothStep(m_CurDir, m_TowardDir, DT * m_RotSpeed));

    // 방향 설정
    // Transform()->SetDirection(m_TowardDir);
}

void CKirbyMoveController::Move()
{
    // 가속도 초기화
    m_Accel = {0.f, 0.f, 0.f};

    bool bGrounded = CharacterController()->IsGrounded();
    RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Transform()->GetWorldPos(), Vec3(0.f, -1.f, 0.f), m_HoveringLimitHeight, {L"Layer 1"});
    
    float a = CharacterController()->GetHeight() / 2.f - CharacterController()->GetRadius();

    if (Hit.pCollisionObj && Hit.Distance <= 0.1f)
    {
        bGrounded = true;
    }
    else
    {
        bGrounded = false;
    }

    if (PLAYERFSM->IsHovering())
    {
        if (Hit.Distance >= CharacterController()->GetHeight() * 2.f)
        {
            PLAYERFSM->SetLastJump(LastJumpType::HIGH);
        }
    }

    // =========================
    // Velocity 계산
    // =========================

    // 수평 방향 이동속도 계산
    // Guard시에는 이전프레임의 이동속도를 남겨 감속시킴

    if (m_bActiveFriction)
    {
        m_Accel.x = -m_MoveVelocity.x * m_Friction;
        m_Accel.z = -m_MoveVelocity.z * m_Friction;

        m_MoveVelocity.x += m_Accel.x;
        m_MoveVelocity.z += m_Accel.z;
    }
    else
    {
        m_MoveVelocity.x = m_MoveDir.x * m_Speed;
        m_MoveVelocity.z = m_MoveDir.z * m_Speed;
    }

    // Jump
    if (m_bJump && !m_bJumpLock)
    {
        m_bJump = false;
        m_MoveVelocity.y = m_JumpPower;
    }

    // 중력 적용
    m_Accel.y += m_Gravity;

    // 수직 방향 이동속도 계산
    m_MoveVelocity.y += m_Accel.y * DT;

    // 땅에 닿은 상태면 Velocity Y값 초기화
    if (bGrounded && m_MoveVelocity.y < 0)
    {
        m_MoveVelocity.y = 0.f;
    }

    if (PLAYERFSM->IsHovering() && m_HoveringHeight > m_HoveringLimitHeight && m_MoveVelocity.y > 0.f)
    {
        m_MoveVelocity.y = 0.f;
    }

    // AddVelocity 적용
    if (m_AddVelocity.Length() != 0.f)
    {
        m_MoveVelocity += m_AddVelocity;
        m_AddVelocity = {0.f, 0.f, 0.f};
    }

    // =========================
    // Velocity Min / Max 확인
    // =========================
    // 땅에 닿은 상태면 Velocity Y값 초기화
    if (bGrounded && m_MoveVelocity.y < 0)
    {
        m_MoveVelocity.y = 0.f;
    }

    if (PLAYERFSM->IsHovering())
    {
        // falling
        if (m_MoveVelocity.y <= m_HoveringMinSpeed)
        {
            m_MoveVelocity.y = m_HoveringMinSpeed;
        }

        // check limit height
        if (Hit.pCollisionObj == nullptr && m_MoveVelocity.y > 0.f)
        {
            //@TODO 레이어이름
            m_MoveVelocity.y = 0.f;
        }
    }

    // 수평 MaxSpeed 제한
    Vec3 HorizontalVel = {m_MoveVelocity.x, 0.f, m_MoveVelocity.z};
    if (HorizontalVel.Length() > m_MaxSpeed)
    {
        HorizontalVel = HorizontalVel.Normalize() * m_MaxSpeed;
        m_MoveVelocity.x = HorizontalVel.x;
        m_MoveVelocity.z = HorizontalVel.z;
    }

    // =========================
    // 움직임 적용
    // =========================
    CharacterController()->Move(m_MoveVelocity * DT);
}

void CKirbyMoveController::SurfaceAlignment()
{
    bool bGrounded = CharacterController()->IsGrounded();
    float GravityVelue = CPhysicsMgr::GetInst()->GetGravity().y;

    RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Transform()->GetWorldPos(), Vec3(0.f, -1.f, 0.f), 2.f, {L"Ground"});

    // Rotate Character
    if (bGrounded)
    {
        Vec3 SurfaceNormal = Hit.Normal;
        SurfaceNormal.Normalize();

        Vec3 ProjectedTowardDir = m_TowardDir - SurfaceNormal * (m_TowardDir.Dot(SurfaceNormal));
        ProjectedTowardDir.Normalize();

        Vec3 Right = SurfaceNormal.Cross(ProjectedTowardDir);
        Right.Normalize();

        Vec3 Front = Right.Cross(SurfaceNormal);
        Front.Normalize();

        // 회전 행렬 생성
        XMMATRIX rotationMatrix =
            XMMATRIX(XMVectorSet(Right.x, Right.y, Right.z, 0.0f), XMVectorSet(SurfaceNormal.x, SurfaceNormal.y, SurfaceNormal.z, 0.0f),
                     XMVectorSet(Front.x, Front.y, Front.z, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));

        XMMATRIX rotationMatrix2 = DirectX::XMMatrixLookToLH(XMVectorZero(), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f));

        float yaw, pitch, roll;
        pitch = asinf(-rotationMatrix.r[2].m128_f32[1]);
        yaw = atan2f(rotationMatrix.r[2].m128_f32[0], rotationMatrix.r[2].m128_f32[2]);
        roll = atan2f(rotationMatrix.r[0].m128_f32[1], rotationMatrix.r[1].m128_f32[1]);

        Transform()->SetRelativeRotation({pitch, yaw, roll});
    }
    else
    {
        Transform()->SetDirection(m_TowardDir);

        Vec3 Movement = {0.f, -50.f, 0.f};

        CharacterController()->Move(Movement * DT);
    }
}

void CKirbyMoveController::OnControllerColliderHit(ControllerColliderHit Hit)
{
    // CPhysicsMgr::RayCast(Transform()->GetWorldPos(), Vec3(0.f,-1.f,0.f), 100.f, )
    int a = 0;
}

void CKirbyMoveController::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Speed, 1, sizeof(float), _File);
    fwrite(&m_Gravity, 1, sizeof(float), _File);
    fwrite(&m_JumpPower, 1, sizeof(float), _File);
}

void CKirbyMoveController::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Speed, 1, sizeof(float), _File);
    fread(&m_Gravity, 1, sizeof(float), _File);
    fread(&m_JumpPower, 1, sizeof(float), _File);
}
