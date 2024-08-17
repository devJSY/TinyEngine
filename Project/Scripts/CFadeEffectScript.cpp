#include "pch.h"
#include "CFadeEffectScript.h"
#include "define.h"
#include <Engine\\CRenderMgr.h>
#include "CPlayerMgr.h"

CFadeEffectScript::CFadeEffectScript()
    : CScript(FADEEFFECTSCRIPT)
    , m_Target(nullptr)
    , m_BackGroundColor()
    , m_bReverse(false)
    , m_bComplete(false)
    , m_Duration(1.f)
    , m_ElapsedTime(0.f)
    , m_RotateSpeed(1.25f)
{
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_BackGroundColor, "BackGroundColor");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bReverse, "Reverse");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotateSpeed, "Rotate Speed");
}

CFadeEffectScript::CFadeEffectScript(const CFadeEffectScript& origin)
    : CScript(origin)
    , m_Target(nullptr)
    , m_BackGroundColor(origin.m_BackGroundColor)
    , m_bReverse(origin.m_bReverse)
    , m_Duration(origin.m_Duration)
    , m_ElapsedTime(0.f)
    , m_RotateSpeed(origin.m_RotateSpeed)
{
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_BackGroundColor, "BackGroundColor");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bReverse, "Reverse");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotateSpeed, "Rotate Speed");
}

CFadeEffectScript::~CFadeEffectScript()
{
}

void CFadeEffectScript::begin()
{
    m_Target = PLAYER;

    m_bComplete = false;

    m_bReverse ? m_ElapsedTime = m_Duration : m_ElapsedTime = 0.f;
}

void CFadeEffectScript::tick()
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
    else if (m_ElapsedTime < -(m_Duration * 10.f))
    {
        m_ElapsedTime = -(m_Duration * 10.f);
        m_bComplete = true;
    }

    if (nullptr != GetOwner()->MeshRender() && nullptr != MeshRender()->GetMaterial(0))
    {
        Vec4 NDCPos = Vec4(-100, -100, -100, -100);
        if (nullptr != m_Target)
        {
            NDCPos = PositionToNDC(m_Target->Transform()->GetWorldPos());

            // Target이 화면 밖인경우 중심으로 설정
            if (NDCPos.x < -1.f || NDCPos.y < -1.f || NDCPos.x > 1.f || NDCPos.y > 1.f)
            {
                NDCPos.x = 0.f;
                NDCPos.y = 0.f;
            }
        }

        MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, m_ElapsedTime / m_Duration);
        MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, XM_PI * m_ElapsedTime * m_RotateSpeed);
        MeshRender()->GetMaterial(0)->SetScalarParam(VEC2_0, Vec2(NDCPos.x, NDCPos.y));
        MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_0, m_BackGroundColor);
    }
}

UINT CFadeEffectScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_BackGroundColor, sizeof(Vec4), 1, _File);
    fwrite(&m_bReverse, sizeof(bool), 1, _File);
    fwrite(&m_Duration, sizeof(float), 1, _File);
    fwrite(&m_RotateSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    return MemoryByte;
}

UINT CFadeEffectScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_BackGroundColor, sizeof(Vec4), 1, _File);
    fread(&m_bReverse, sizeof(bool), 1, _File);
    fread(&m_Duration, sizeof(float), 1, _File);
    fread(&m_RotateSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    return MemoryByte;
}
