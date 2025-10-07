#include "pch.h"
#include "CModelEditorCameraMoveScript.h"

#include <Engine/CEditorMgr.h>
#include <Engine/CLevelEditor.h>
#include <Engine/CDevice.h>
#include <Engine/CLevelMgr.h>

CModelEditorCameraMoveScript::CModelEditorCameraMoveScript()
    : CScript(MODELEDITORCAMERAMOVESCRIPT)
    , m_CamSpeed(10.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CamSpeed, "Camera Speed");
}

CModelEditorCameraMoveScript::CModelEditorCameraMoveScript(const CModelEditorCameraMoveScript& origin)
    : CScript(origin)
    , m_CamSpeed(origin.m_CamSpeed)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CamSpeed, "Camera Speed");
}

CModelEditorCameraMoveScript::~CModelEditorCameraMoveScript()
{
}

void CModelEditorCameraMoveScript::tick()
{
    if (nullptr == Camera())
        return;

    // ��ġ �ʱ�ȭ
    if (KEY_TAP(KEY::F))
    {
        Transform()->SetLocalPos(Vec3(0.f, 250.f, -250.f));
        Transform()->SetLocalRotation(Vec3(DirectX::XMConvertToRadians(15.f), 0.f, 0.f));
    }

    // Move
    if (CEditorMgr::GetInst()->GetModelEditor()->IsViewportHovered())
    {
        MovePerspective();
    }

    // ī�޶� �ӵ� ����
    if (m_CamSpeed < 0.f)
        m_CamSpeed = 0.f;
}

void CModelEditorCameraMoveScript::MovePerspective()
{
    if (KEY_PRESSED(KEY::RBTN))
    {
        // Move
        Vec3 vPos = Transform()->GetLocalPos();

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

        Transform()->SetLocalPos(vPos);

        // Drag
        Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();
        Vec3 vRot = Transform()->GetLocalRotation();
        vRot.y += vDrag.x * XM_PI / 360.f;
        vRot.x += vDrag.y * XM_PI / 360.f;
        Transform()->SetLocalRotation(vRot);

        // Camera speed
        short wheel = CKeyMgr::GetInst()->GetMouseWheel();
        if (wheel > 0)
            m_CamSpeed *= 1.1f;
        else if (wheel < 0)
            m_CamSpeed *= 0.9f;
    }

    // Zoom
    float Zoffset = 1.5f;
    if (KEY_PRESSED(KEY::B))
        Camera()->SetFOV(Camera()->GetFOV() + DT_ENGINE * Zoffset);

    if (KEY_PRESSED(KEY::M))
        Camera()->SetFOV(Camera()->GetFOV() - DT_ENGINE * Zoffset);
}

UINT CModelEditorCameraMoveScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_CamSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CModelEditorCameraMoveScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_CamSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}