#include "pch.h"
#include "CCameraMoveScript.h"
#include "CEditorMgr.h"
#include "CLevelEditor.h"

CCameraMoveScript::CCameraMoveScript()
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::tick()
{
    // Projection Change
    if (KEY_TAP(KEY::P))
    {
        if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
            Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
        else
            Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

        Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
    }

    // Level 에디터일 경우 Viewport창에서만 카메라 이동 적용
    CLevelEditor* LevelDditor = dynamic_cast<CLevelEditor*>(CEditorMgr::GetInst()->GetCurEditor());
    if (nullptr != LevelDditor)
    {
        if (!LevelDditor->IsViewportHovered())
            return;
    }

    // Move
    if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
        MoveOrthographic();
    else
        MovePerspective();

    // Wheel 초기화
    CKeyMgr::GetInst()->SetMouseWheel(0);
}

void CCameraMoveScript::MoveOrthographic()
{
    float CamSpeed = GetOwner()->Camera()->GetCameraSpeed();

    // Move
    Vec3 vPos = Transform()->GetRelativePos();

    if (KEY_PRESSED(KEY::W))
        vPos.y += DT * CamSpeed;

    if (KEY_PRESSED(KEY::S))
        vPos.y -= DT * CamSpeed;

    if (KEY_PRESSED(KEY::A))
        vPos.x -= DT * CamSpeed;

    if (KEY_PRESSED(KEY::D))
        vPos.x += DT * CamSpeed;

    Transform()->SetRelativePos(vPos);

    // Drag
    if (KEY_PRESSED(KEY::RBTN))
    {
        Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();
        vDrag.Normalize();
        Vec3 vPos = Transform()->GetRelativePos();
        float scale = Camera()->GetScale();
        float Doffset = 4.f;
        vPos.x -= vDrag.x * DT * CamSpeed * scale * Doffset;
        vPos.y += vDrag.y * DT * CamSpeed * scale * Doffset;
        Transform()->SetRelativePos(vPos);
    }

    // Zoom
    short wheel = CKeyMgr::GetInst()->GetMouseWheel();
    float Zoffset = 100.f;
    if (wheel < 0)
        Camera()->SetScale(Camera()->GetScale() + DT * Zoffset);
    else if (wheel > 0)
        Camera()->SetScale(Camera()->GetScale() - DT * Zoffset);
}

void CCameraMoveScript::MovePerspective()
{
    float CamSpeed = GetOwner()->Camera()->GetCameraSpeed();

    // Move
    Vec3 vPos = Transform()->GetRelativePos();

    Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
    Vec3 vUp = Transform()->GetWorldDir(DIR_TYPE::UP);

    if (KEY_PRESSED(KEY::W))
        vPos += DT * CamSpeed * vFront;

    if (KEY_PRESSED(KEY::S))
        vPos += DT * CamSpeed * -vFront;

    if (KEY_PRESSED(KEY::A))
        vPos += DT * CamSpeed * -vRight;

    if (KEY_PRESSED(KEY::D))
        vPos += DT * CamSpeed * vRight;

    if (KEY_PRESSED(KEY::E))
        vPos += DT * CamSpeed * vUp;

    if (KEY_PRESSED(KEY::Q))
        vPos += DT * CamSpeed * -vUp;

    Transform()->SetRelativePos(vPos);

    // Drag
    if (KEY_PRESSED(KEY::RBTN))
    {
        Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();
        vDrag.Normalize();
        Vec3 vRot = Transform()->GetRelativeRotation();
        float Doffset = 4.f;
        vRot.y += vDrag.x * DT * XM_PI * Doffset;
        vRot.x += vDrag.y * DT * XM_PI * Doffset;
        Transform()->SetRelativeRotation(vRot);

        // Camera speed
        short wheel = CKeyMgr::GetInst()->GetMouseWheel();
        float CamSpeedOffset = 10000.f;
        if (wheel < 0)
            Camera()->SetCameraSpeed(CamSpeed - DT * CamSpeedOffset);
        else if (wheel > 0)
            Camera()->SetCameraSpeed(CamSpeed + DT * CamSpeedOffset);
    }

    // Zoom
    float Zoffset = 2.f;
    if (KEY_PRESSED(KEY::Z))
        Camera()->SetFOV(Camera()->GetFOV() + DT * Zoffset);

    if (KEY_PRESSED(KEY::C))
        Camera()->SetFOV(Camera()->GetFOV() - DT * Zoffset);
}
