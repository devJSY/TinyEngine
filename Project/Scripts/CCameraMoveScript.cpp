#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine\\CEditorMgr.h>
#include <Engine\\CLevelEditor.h>
#include <Engine\\CDevice.h>
#include <Engine\\CLevelMgr.h>

CCameraMoveScript::CCameraMoveScript()
    : CScript(CAMERAMOVESCRIPT)
    , m_CamSpeed(250.f)
    , m_bFocus(false)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::tick()
{
    if (nullptr == Camera())
        return;

    // ������ ���
    if (CEditorMgr::GetInst()->IsEnable())
    {
        // Projection Change
        if (KEY_TAP(KEY::P))
        {
            if (CEditorMgr::GetInst()->GetLevelEditor()->IsViewportFocused())
            {
                if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
                    Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
                else
                    Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

                // Rotation �ʱ�ȭ
                Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
            }
        }

        // Selected Obejct Focus
        if (KEY_TAP(KEY::F))
            m_bFocus = true;

        if (m_bFocus)
        {
            if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
                MoveFocusOrthographic();
            else
                MoveFocusPerspective();
        }

        // ���� ������ ������ Viewportâ������ ī�޶� �̵� ����
        if (!CEditorMgr::GetInst()->GetLevelEditor()->IsViewportHovered())
            return;

        // Move
        if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
            MoveOrthographic();
        else
            MovePerspective();
    }
    else
    {
        // �÷��� ���

        // Projection Change
        if (KEY_TAP(KEY::P))
        {
            if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
                Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
            else
                Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

            // Rotation �ʱ�ȭ
            Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
        }

        // Move
        if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
            MoveOrthographic();
        else
            MovePerspective();
    }

    // Wheel �ʱ�ȭ
    CKeyMgr::GetInst()->SetMouseWheel(0);

    // ī�޶� �ӵ� ����
    if (m_CamSpeed < 0.f)
        m_CamSpeed = 0.f;
}

void CCameraMoveScript::MoveOrthographic()
{
    // Drag
    if (KEY_PRESSED(KEY::RBTN))
    {
        Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();
        float scale = Camera()->GetScale();

        Vec2 ViewportSize = CEditorMgr::GetInst()->GetViewportSize();
        Vec2 RenderResolution = CDevice::GetInst()->GetRenderResolution();

        float xRatio = 0.f;
        float yRatio = 0.f;

        if (ViewportSize.x == 0 || ViewportSize.y == 0 || !CEditorMgr::GetInst()->IsEnable())
        {
            xRatio = 1.f;
            yRatio = 1.f;
        }
        else
        {
            xRatio = RenderResolution.x / ViewportSize.x;
            yRatio = RenderResolution.y / ViewportSize.y;
        }

        // �巡���� �ȼ� ��ġ����ŭ �̵�
        // ����Ʈ ���� ����ؼ� ����
        Vec3 vPos = Transform()->GetRelativePos();
        vPos.x -= vDrag.x * scale * xRatio;
        vPos.y += vDrag.y * scale * yRatio;
        Transform()->SetRelativePos(vPos);
    }

    // Zoom
    short wheel = CKeyMgr::GetInst()->GetMouseWheel();
    if (wheel < 0)
        Camera()->SetScale(Camera()->GetScale() * 1.1f);
    else if (wheel > 0)
        Camera()->SetScale(Camera()->GetScale() * 0.9f);
}

void CCameraMoveScript::MovePerspective()
{
    if (KEY_PRESSED(KEY::RBTN))
    {
        // Move
        Vec3 vPos = Transform()->GetRelativePos();

        Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
        Vec3 vUp = Transform()->GetWorldDir(DIR_TYPE::UP);

        if (KEY_PRESSED(KEY::W))
            vPos += DT_ENGINE * m_CamSpeed * vFront;

        if (KEY_PRESSED(KEY::S))
            vPos += DT_ENGINE * m_CamSpeed * -vFront;

        if (KEY_PRESSED(KEY::A))
            vPos += DT_ENGINE * m_CamSpeed * -vRight;

        if (KEY_PRESSED(KEY::D))
            vPos += DT_ENGINE * m_CamSpeed * vRight;

        if (KEY_PRESSED(KEY::E))
            vPos += DT_ENGINE * m_CamSpeed * vUp;

        if (KEY_PRESSED(KEY::Q))
            vPos += DT_ENGINE * m_CamSpeed * -vUp;

        Transform()->SetRelativePos(vPos);

        // Drag
        Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();
        Vec3 vRot = Transform()->GetRelativeRotation();
        vRot.y += vDrag.x * XM_PI / 360.f;
        vRot.x += vDrag.y * XM_PI / 360.f;
        Transform()->SetRelativeRotation(vRot);

        // Camera speed
        short wheel = CKeyMgr::GetInst()->GetMouseWheel();
        if (wheel > 0)
            m_CamSpeed *= 1.1f;
        else if (wheel < 0)
            m_CamSpeed *= 0.9f;
    }

    // Zoom
    float Zoffset = 1.5f;
    if (KEY_PRESSED(KEY::Z))
        Camera()->SetFOV(Camera()->GetFOV() + DT_ENGINE * Zoffset);

    if (KEY_PRESSED(KEY::C))
        Camera()->SetFOV(Camera()->GetFOV() - DT_ENGINE * Zoffset);
}

void CCameraMoveScript::MoveFocusOrthographic()
{
    CGameObject* pSelectedObj = CEditorMgr::GetInst()->GetSelectedObject();

    if (nullptr == pSelectedObj)
    {
        m_bFocus = false;
        return;
    }

    Vec3 CamPos = Transform()->GetWorldPos();
    Vec3 ObjPos = pSelectedObj->Transform()->GetWorldPos();

    Vec2 dir = Vec2(ObjPos.x, ObjPos.y) - Vec2(CamPos.x, CamPos.y);

    // ī�޶� ������Ʈ�� ���õǾ��ų� ������ ��ġ��ó�� �����ߴٸ� Stop
    if (GetOwner() == pSelectedObj || (dir.Length() < 10.f))
    {
        m_bFocus = false;
    }

    if (m_bFocus)
    {
        Vec3 dir3 = Vec3(dir.x, dir.y, 0.f);
        float CamMoveSpeed = dir.Length() * 25.f;
        float scale = Camera()->GetScale();
        Transform()->SetRelativePos(Transform()->GetRelativePos() + DT_ENGINE * dir3.Normalize() * CamMoveSpeed * scale);
    }
}

void CCameraMoveScript::MoveFocusPerspective()
{
    CGameObject* pSelectedObj = CEditorMgr::GetInst()->GetSelectedObject();

    if (nullptr == pSelectedObj)
    {
        m_bFocus = false;
        return;
    }

    float focusDist = 200.f;
    Vec3 CamDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 CamDistPos = Transform()->GetWorldPos() + CamDir * focusDist;

    Vec3 ObjPos = pSelectedObj->Transform()->GetWorldPos();
    Vec3 dir = ObjPos - CamDistPos;

    // ī�޶� ������Ʈ�� ���õǾ��ų� ������ ��ġ��ó�� �����ߴٸ� Stop
    if (GetOwner() == pSelectedObj || (dir.Length() < 10.f))
    {
        m_bFocus = false;
    }

    if (m_bFocus)
    {
        float CamMoveSpeed = dir.Length() * 25.f;
        Transform()->SetRelativePos(Transform()->GetRelativePos() + DT_ENGINE * dir.Normalize() * CamMoveSpeed);
    }
}

void CCameraMoveScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_CamSpeed, sizeof(float), 1, _File);
}

void CCameraMoveScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_CamSpeed, sizeof(float), 1, _File);
}