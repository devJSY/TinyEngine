#include "pch.h"
#include "CFadeOutScript.h"
#include "define.h"
#include <Engine\\CRenderMgr.h>
#include "CPlayerMgr.h"

CFadeOutScript::CFadeOutScript()
    : CScript(FADEOUTSCRIPT)
    , m_Target(nullptr)
    , m_bReverse(false)
    , m_bComplete(false)
    , m_Duration(1.f)
    , m_ElapsedTime(0.f)
    , m_RotateSpeed(1.25f)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bReverse, "Reverse");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotateSpeed, "Rotate Speed");
}

CFadeOutScript::CFadeOutScript(const CFadeOutScript& origin)
    : CScript(origin)
    , m_Target(nullptr)
    , m_bReverse(origin.m_bReverse)
    , m_Duration(origin.m_Duration)
    , m_ElapsedTime(0.f)
    , m_RotateSpeed(origin.m_RotateSpeed)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bReverse, "Reverse");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotateSpeed, "Rotate Speed");
}

CFadeOutScript::~CFadeOutScript()
{
}

void CFadeOutScript::begin()
{
    m_Target = PLAYER;

    m_bComplete = false;
    m_bReverse ? m_ElapsedTime = m_Duration : m_ElapsedTime = 0.f;
}

void CFadeOutScript::tick()
{
    if (m_bComplete)
        return;

    if (nullptr == m_Target)
    {
        m_Target = PLAYER;
        return;
    }

    m_bReverse ? m_ElapsedTime -= DT : m_ElapsedTime += DT;

    if (m_ElapsedTime > m_Duration)
    {
        m_ElapsedTime = m_Duration;
        m_bComplete = true;
    }
    else if (m_ElapsedTime < -(m_Duration * 5.f))
    {
        m_ElapsedTime = -(m_Duration * 5.f);
        m_bComplete = true;
    }

    if (nullptr != GetOwner()->MeshRender() && nullptr != MeshRender()->GetMaterial(0))
    {
        Vec4 NDCPos = Vec4(-100, -100, -100, -100);
        if (nullptr != m_Target)
        {
            NDCPos = PositionToNDC(m_Target->Transform()->GetWorldPos());
        }
        MeshRender()->GetMaterial(0)->SetScalarParam(VEC2_0, Vec2(NDCPos.x, NDCPos.y));
        MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, m_ElapsedTime / m_Duration);
        MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, XM_PI * m_ElapsedTime * m_RotateSpeed);
    }
}

UINT CFadeOutScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bReverse, sizeof(bool), 1, _File);
    fwrite(&m_Duration, sizeof(float), 1, _File);
    fwrite(&m_RotateSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    return MemoryByte;
}

UINT CFadeOutScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bReverse, sizeof(bool), 1, _File);
    fread(&m_Duration, sizeof(float), 1, _File);
    fread(&m_RotateSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    return MemoryByte;
}
