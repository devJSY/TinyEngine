#include "pch.h"
#include "CKirbyMoveController.h"

#include <Engine/CRenderMgr.h>

CKirbyMoveController::CKirbyMoveController()
    : CScript(KIRBYMOVECONTROLLER)
    , m_Dir{}
    , m_LookDir{}
    , m_Move(false)
{
}

CKirbyMoveController::CKirbyMoveController(const CKirbyMoveController& _Origin)
    : CScript(_Origin)
    , m_Dir{}
    , m_LookDir{}
    , m_Move(false)
{
}

CKirbyMoveController::~CKirbyMoveController()
{
}

void CKirbyMoveController::begin()
{
}

void CKirbyMoveController::tick()
{
    SetDir();

    if (m_Move)
    {
        Vec3 Dir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Vec3 Pos = Transform()->GetRelativePos();

        Transform()->SetRelativePos(Pos + Dir * DT * 20);
    }
}

void CKirbyMoveController::SetDir()
{
    // Main Camera Front 가져오기
    CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();

    if (nullptr == pCam)
        return;

    Vec3 CamRotation = pCam->Transform()->GetRelativeRotation();

    Vec3 CamDir = pCam->Transform()->GetLocalDir(DIR_TYPE::FRONT);
    CamDir.z = 0.f;
    CamDir.Normalize();

    // 키 입력 확인
    Vec3 InputDir = Vec3(0.f, 0.f, 0.f);

    // ↑ ↓
    if (KEY_TAP(KEY::W) || KEY_PRESSED(KEY::W))
    {
        InputDir += Vec3(0.f, 0.f, 1.f);
    }
    if (KEY_TAP(KEY::S) || KEY_PRESSED(KEY::S))
    {
        InputDir += Vec3(0.f, 0.f, -1.f);
    }

    // ← →
    if (KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D))
    {
        InputDir += Vec3(1.f, 0.f, 0.f);
    }
    if (KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A))
    {
        InputDir += Vec3(-1.f, 0.f, 0.f);
    }

    // 키 입력이 있다면 바라봐야할 방향을 다시 갱신한다
    if (InputDir.Length() != 0.f)
    {
        InputDir.Normalize();

        // Camera Y축 회전 행렬 만들기
        Matrix matRotY = XMMatrixIdentity();
        matRotY = XMMatrixRotationY(CamRotation.y);

        // 입력 방향에 카메라 y축 회전을 더해 World 기준 방향으로 변환
        m_LookDir = XMVector3TransformNormal(InputDir, matRotY);

        m_Move = true;
    }
    else
    {
        m_Move = false;
    }

    // 구면보간을 이용해서 물체의 새로운 방향을 정의
    Transform()->SetDirection(Vector3::SmoothStep(Transform()->GetWorldDir(DIR_TYPE::FRONT), m_LookDir, DT * 50.f));

    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), 50.f, Vec3(0.f, 1.f, 0.f), false);
}

void CKirbyMoveController::SaveToLevelFile(FILE* _File)
{
}

void CKirbyMoveController::LoadFromLevelFile(FILE* _File)
{
}
