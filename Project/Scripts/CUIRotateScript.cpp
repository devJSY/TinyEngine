#include "pch.h"
#include "CUIRotateScript.h"

CUIRotateScript::CUIRotateScript()
    : CUIAnimScript(UIROTATESCRIPT)
    , m_fSpeed(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Rotate Speed");
}

CUIRotateScript::CUIRotateScript(const CUIRotateScript& Origin)
    : CUIAnimScript(Origin)
    , m_fSpeed(Origin.m_fSpeed)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Rotate Speed");
}

CUIRotateScript::~CUIRotateScript()
{
}

void CUIRotateScript::begin()
{
    SetLoopUI(true);
}

void CUIRotateScript::tick()
{
    Rotate();
}

void CUIRotateScript::Rotate()
{
    Vec3 vRot = Transform()->GetLocalRotation();

    vRot.z += m_fSpeed * DT;

    Transform()->SetLocalRotation(vRot);
}

UINT CUIRotateScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CUIRotateScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}