#include "pch.h"
#include "CUIGlowMoveScript.h"

CUIGlowMoveScript::CUIGlowMoveScript()
    : CUIAnimScript(UIGLOWMOVESCRIPT)
    , m_fStartOff(0.f)
    , m_fDist(0.f)
    , m_fSpeed(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDist, "DistOffset");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "SpeedOffset");
}

CUIGlowMoveScript::CUIGlowMoveScript(const CUIGlowMoveScript& Origin)
    : CUIAnimScript(Origin)
    , m_fStartOff(0.f)
    , m_fDist(Origin.m_fDist)
    , m_fSpeed(Origin.m_fSpeed)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDist, "DistOffset");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "SpeedOffset");
}

CUIGlowMoveScript::~CUIGlowMoveScript()
{
}

void CUIGlowMoveScript::tick()
{
    CUIAnimScript::tick();

    if (UIAnimState::Start == GetUIAnimState())
    {
        SetFinish(false);
        SetUIAnimState(UIAnimState::Tick);
    }

    MoveGlowRatio();
}

void CUIGlowMoveScript::MoveGlowRatio()
{
    if (UIAnimState::Tick == GetUIAnimState())
    {
        m_fStartOff += DT * m_fSpeed;

        GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_2, m_fStartOff);
        GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_3, m_fStartOff + m_fDist);

        if (m_fStartOff >= 1.f)
        {
            GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_2, 0.f);
            GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_3, 0.f);

            m_fStartOff = 0.f;
            SetUIAnimState(UIAnimState::End);
            SetFinish(true);
        }
    }
}

UINT CUIGlowMoveScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fDist, sizeof(float), 1, _File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);

    MemoryByte += CUIAnimScript::SaveToLevelFile(_File);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CUIGlowMoveScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fDist, sizeof(float), 1, _File);
    fread(&m_fSpeed, sizeof(float), 1, _File);

    MemoryByte += CUIAnimScript::LoadFromLevelFile(_File);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
