#include "pch.h"
#include "CKirbyVacuumScript.h"

CKirbyVacuumScript::CKirbyVacuumScript()
    : CScript(KIRBYVACUUMSCRIPT)
    , m_InsideVacuum(nullptr)
    , m_OutsdieVacuum(nullptr)
    , m_Acc(0.f)
    , m_ScalingDuration(0.5f)
    , m_StartColor(Vec4(300.f,170.f,200.f,255.f))
    , m_EndColor(Vec4(170.f,200.f,300.f,255.f))
    , m_StartGradationAlpha(0.5f)
    , m_EndGradationAlpha(0.2f)
    , m_RotationSpeed(180.f)
    , m_InitColor(Vec4(255.f,255.f,255.f,255.f))
    , m_Frequency(1.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ScalingDuration, "Scaling Duration", 0.1f);

    AddScriptParam(SCRIPT_PARAM::VEC4, &m_StartColor, "Start Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_EndColor, "Start Color");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_StartGradationAlpha, "Start Gradation Alpha", 0.1f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_EndGradationAlpha, "End Gradation Alpha", 0.1f);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotationSpeed, "Rotation Speed(Degree)");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Frequency, "Frequency");

}

CKirbyVacuumScript::CKirbyVacuumScript(const CKirbyVacuumScript& _Origin)
    : CScript(KIRBYVACUUMSCRIPT)
    , m_InsideVacuum(nullptr)
    , m_OutsdieVacuum(nullptr)
    , m_Acc(0.f)
    , m_ScalingDuration(_Origin.m_ScalingDuration)
    , m_StartColor(_Origin.m_StartColor)
    , m_EndColor(_Origin.m_EndColor)
    , m_StartGradationAlpha(_Origin.m_StartGradationAlpha)
    , m_EndGradationAlpha(_Origin.m_EndGradationAlpha)
    , m_RotationSpeed(_Origin.m_RotationSpeed)
    , m_InitColor(Vec4(255.f, 255.f, 255.f, 255.f))
    , m_Frequency(_Origin.m_Frequency)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ScalingDuration, "Scaling Duration", 0.1f);

    AddScriptParam(SCRIPT_PARAM::VEC4, &m_StartColor, "Start Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_EndColor, "Start Color");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_StartGradationAlpha, "Start Gradation Alpha", 0.1f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_EndGradationAlpha, "End Gradation Alpha", 0.1f);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotationSpeed, "Rotation Speed(Degree)");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Frequency, "Frequency");
}

CKirbyVacuumScript::~CKirbyVacuumScript()
{
}


void CKirbyVacuumScript::begin()
{
    m_InsideVacuum = GetOwner()->GetChildObject(L"VacuumInside");
    m_OutsdieVacuum = GetOwner()->GetChildObject(L"VacuumOutside");
    m_InitColor = Vec4(255.f, 255.f, 255.f, 255.f);

    if (m_InsideVacuum == nullptr || m_OutsdieVacuum == nullptr)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }

    Reset();
}

void CKirbyVacuumScript::tick()
{
    if (m_InsideVacuum == nullptr || m_OutsdieVacuum == nullptr)
        return;

    m_Acc += DT;

    // Vacuum이 지속되면서 크기가 커진다.
    {
        float Ratio = clamp((m_Acc / m_ScalingDuration), 0.f, 1.f);
        float CurGradationAlpha = Lerp(m_StartGradationAlpha, m_EndGradationAlpha, Ratio);

        m_InsideVacuum->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, CurGradationAlpha);
        m_OutsdieVacuum->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, CurGradationAlpha);
    }


    // 회전
    {
        Vec3 CurRotation = Transform()->GetLocalRotation();
        CurRotation.z += (m_RotationSpeed * XM_PI / 180.f) * DT;
        Transform()->SetLocalRotation(CurRotation);
    }


    // 색 변경
    if (m_Acc < m_ScalingDuration)
    {
        float Ratio = (m_Acc / m_ScalingDuration) * XM_PI * 0.25f;

        Vec4 CurColor = m_InitColor + (m_StartColor - m_InitColor) * sinf(Ratio);
        CurColor /= 255.f;

        m_InsideVacuum->MeshRender()->GetMaterial(0)->SetAlbedo(CurColor);
        m_OutsdieVacuum->MeshRender()->GetMaterial(0)->SetAlbedo(CurColor);

    }
    else if (m_Acc > m_ScalingDuration)
    {
        float CurAcc = m_ScalingDuration - m_Acc;
        float Ratio = CurAcc / m_Frequency;

        Vec4 CurColor = m_StartColor + (m_EndColor - m_StartColor) * 0.5f * (1.0f + sinf(2.0f * XM_PI * Ratio));
        CurColor /= 255.f;

        m_InsideVacuum->MeshRender()->GetMaterial(0)->SetAlbedo(CurColor);
        m_OutsdieVacuum->MeshRender()->GetMaterial(0)->SetAlbedo(CurColor);
    }

}

void CKirbyVacuumScript::Reset()
{
    if (m_InsideVacuum == nullptr || m_OutsdieVacuum == nullptr)
        return;

    m_InsideVacuum->MeshRender()->GetMaterial(0)->SetAlbedo(m_InitColor);
    m_OutsdieVacuum->MeshRender()->GetMaterial(0)->SetAlbedo(m_InitColor);

    
    m_InsideVacuum->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, m_StartGradationAlpha);
    m_OutsdieVacuum->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, m_StartGradationAlpha);

    Transform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));

    m_Acc = 0.f;
}

UINT CKirbyVacuumScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_ScalingDuration, sizeof(float), 1, _File);

    fwrite(&m_StartColor, sizeof(Vec4), 1, _File);
    fwrite(&m_EndColor, sizeof(Vec4), 1, _File);

    fwrite(&m_StartGradationAlpha, sizeof(float), 1, _File);
    fwrite(&m_EndGradationAlpha, sizeof(float), 1, _File);

    fwrite(&m_RotationSpeed, sizeof(float), 1, _File);
    fwrite(&m_Frequency, sizeof(float), 1, _File);


    MemoryByte += sizeof(float);
    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CKirbyVacuumScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_ScalingDuration, sizeof(float), 1, _File);

    fread(&m_StartColor, sizeof(Vec4), 1, _File);
    fread(&m_EndColor, sizeof(Vec4), 1, _File);

    fread(&m_StartGradationAlpha, sizeof(float), 1, _File);
    fread(&m_EndGradationAlpha, sizeof(float), 1, _File);

    fread(&m_RotationSpeed, sizeof(float), 1, _File);
    fread(&m_Frequency, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
