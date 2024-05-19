#include "pch.h"
#include "CPlayerScript.h"
#include <Engine\\CPhysicsMgr.h>

CPlayerScript::CPlayerScript()
    : CScript(PLAYERSCRIPT)
    , m_MoveVelocity(Vec3())
    , m_Speed(2.f)
    , m_ForcePower(10.f)
    , m_JumpPower(1.f)
    , m_RayCastDist(15.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ForcePower, "Force Power");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpPower, "Jump Power");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RayCastDist, "RayCast Distance");
}

CPlayerScript::CPlayerScript(const CPlayerScript& origin)
    : CScript(origin)
    , m_MoveVelocity(Vec3())
    , m_Speed(origin.m_Speed)
    , m_ForcePower(origin.m_ForcePower)
    , m_JumpPower(origin.m_JumpPower)
    , m_RayCastDist(origin.m_JumpPower)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ForcePower, "Force Power");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpPower, "Jump Power");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RayCastDist, "RayCast Distance");
}

CPlayerScript::~CPlayerScript()
{
}
void CPlayerScript::begin()
{
    // 플레이어는 캐릭터 컨트롤러 컴포넌트를 무조건 소유하도록 추가
    if (nullptr == CharacterController())
    {
        GetOwner()->AddComponent(new CCharacterController);
    }
}

void CPlayerScript::tick()
{
    bool bGrounded = CharacterController()->IsGrounded();
    float GravityVelue = CPhysicsMgr::GetInst()->GetGravity().y;

    // CharacterController의 Grounded 는 마지막 Move()를 기준으로 저장되기 때문에 정확도가 낮음
    // Raycast로 한번 더 땅에 닿은 상태인지 확인한다.
    if (!bGrounded)
    {
        RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Transform()->GetWorldPos(), Vec3(0.f, -1.f, 0.f), m_RayCastDist, {L"Ground"});
        bGrounded = nullptr != Hit.pCollisionObj;
    }

    // 땅에 닿은 상태면 Velocity Y값 초기화
    if (bGrounded && m_MoveVelocity.y < 0)
    {
        m_MoveVelocity.y = 0.f;
    }

    // ↑ ↓
    float Vertical = 0.f;
    if (KEY_TAP(KEY::W) || KEY_PRESSED(KEY::W))
    {
        Vertical += 1.f;
    }
    if (KEY_TAP(KEY::S) || KEY_PRESSED(KEY::S))
    {
        Vertical -= 1.f;
    }

    // ← →
    float Horizontal = 0.f;
    if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
    {
        Horizontal += 1.f;
    }
    if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
    {
        Horizontal -= 1.f;
    }

    // 이동
    Vec3 Dir = Vec3(Horizontal, 0, Vertical);
    CharacterController()->Move(Dir * m_Speed * DT);

    // 방향 전환
    if (Dir.Length() > 0.f)
    {
        Transform()->SetDirection(Dir);
    }

    // 점프
    if (KEY_TAP(KEY::SPACE) && bGrounded)
    {
        m_MoveVelocity.y += m_JumpPower - GravityVelue;
    }

    // 중력 적용
    m_MoveVelocity.y += GravityVelue * DT;
    CharacterController()->Move(m_MoveVelocity * DT);
}

void CPlayerScript::OnControllerColliderHit(ControllerColliderHit Hit)
{
    if (L"Ball" == Hit.Collider->GetOwner()->GetName())
    {
        if (Hit.Collider->Rigidbody())
        {
            Hit.Collider->Rigidbody()->AddForce(Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_ForcePower, ForceMode::Force);
        }
    }
}

void CPlayerScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Speed, 1, sizeof(float), _File);
    fwrite(&m_ForcePower, 1, sizeof(float), _File);
    fwrite(&m_JumpPower, 1, sizeof(float), _File);
    fwrite(&m_RayCastDist, 1, sizeof(float), _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Speed, 1, sizeof(float), _File);
    fread(&m_ForcePower, 1, sizeof(float), _File);
    fread(&m_JumpPower, 1, sizeof(float), _File);
    fread(&m_RayCastDist, 1, sizeof(float), _File);
}
