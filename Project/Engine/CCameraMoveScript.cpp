#include "pch.h"
#include "CCameraMoveScript.h"

CCameraMoveScript::CCameraMoveScript()
    : m_CamSpeed(500.f)
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
        {
            Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
            Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
        }
    }

    // Move
    if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
        MoveOrthographic();
    else
        MovePerspective();
}

void CCameraMoveScript::MoveOrthographic()
{
    // Move
    Vec3 vPos = Transform()->GetRelativePos();

    if (KEY_PRESSED(KEY::W))
        vPos.y += DT * m_CamSpeed;

    if (KEY_PRESSED(KEY::S))
        vPos.y -= DT * m_CamSpeed;

    if (KEY_PRESSED(KEY::A))
        vPos.x -= DT * m_CamSpeed;

    if (KEY_PRESSED(KEY::D))
        vPos.x += DT * m_CamSpeed;

    Transform()->SetRelativePos(vPos);

    // Zoom
    if (KEY_PRESSED(KEY::Z))
        Camera()->SetScale(Camera()->GetScale() + DT * 0.2f);

    if (KEY_PRESSED(KEY::C))
        Camera()->SetScale(Camera()->GetScale() - DT * 0.2f);
}

void CCameraMoveScript::MovePerspective()
{
    // Move
    Vec3 vPos = Transform()->GetRelativePos();

    Vec3 vFront = Transform()->GetDir(DIR_TYPE::FRONT);
    Vec3 vRight = Transform()->GetDir(DIR_TYPE::RIGHT);
    Vec3 vUp = Transform()->GetDir(DIR_TYPE::UP);

    if (KEY_PRESSED(KEY::W))
        vPos += DT * m_CamSpeed * vFront;

    if (KEY_PRESSED(KEY::S))
        vPos += DT * m_CamSpeed * -vFront;

    if (KEY_PRESSED(KEY::A))
        vPos += DT * m_CamSpeed * -vRight;

    if (KEY_PRESSED(KEY::D))
        vPos += DT * m_CamSpeed * vRight;

    if (KEY_PRESSED(KEY::E))
        vPos += DT * m_CamSpeed * vUp;

    if (KEY_PRESSED(KEY::Q))
        vPos += DT * m_CamSpeed * -vUp;

    Transform()->SetRelativePos(vPos);

    // Drag
    if (KEY_PRESSED(KEY::RBTN))
    {
        Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();
        Vec3 vRot = Transform()->GetRelativeRotation();
        vRot.y += vDrag.x * DT * XM_PI * 4.f;
        vRot.x += vDrag.y * DT * XM_PI * 4.f;
        Transform()->SetRelativeRotation(vRot);
    }

    // Zoom
    if (KEY_PRESSED(KEY::Z))
        Camera()->SetFOV(Camera()->GetFOV() + DT * 2.f);

    if (KEY_PRESSED(KEY::C))
        Camera()->SetFOV(Camera()->GetFOV() - DT * 2.f);
}
