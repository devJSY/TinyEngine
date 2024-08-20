#include "pch.h"
#include "CKirbyLightScript.h"

CKirbyLightScript::CKirbyLightScript()
    : CScript(KIRBYLIGHTSCRIPT)
    , m_MinFallOfEnd(60.f)
    , m_MaxFallOfEnd(155.f)
    , m_FallRatio(0.774f)
    , m_TurnOnTime(0.1f)
    , m_TurnOffTime(0.6f)
    , m_AccTime(0.f)
    , m_State(KirbyLightState::NONE)
{
}

CKirbyLightScript::~CKirbyLightScript()
{
}

void CKirbyLightScript::begin()
{
    if (!GetOwner()->Light())
    {
        MessageBox(nullptr, L"Light 컴포넌트 없음, 직접 추가 권장", L"Failed To Find Light (KirbyLightScript)", MB_OK);
        GetOwner()->AddComponent(new CLight);
    }
}

void CKirbyLightScript::tick()
{
    if (m_State == KirbyLightState::NONE)
        return;

    m_AccTime += DT;

    if (m_State == KirbyLightState::TurningOn)
    {
        float t = clamp(m_AccTime / m_TurnOnTime, 0.f, 1.f);
        float NewRadius = m_MinFallOfEnd + (m_MaxFallOfEnd - m_MinFallOfEnd) * t;

        GetOwner()->Light()->SetFallOffEnd(NewRadius);
        GetOwner()->Light()->SetFallOffStart(NewRadius * m_FallRatio);
        GetOwner()->Light()->SetLightRadiance(Vec3(1000.f, 1000.f, 1000.f) / 255.f);

        if (m_AccTime > m_TurnOnTime)
        {
            m_State = KirbyLightState::NONE;
        }
    }
    else if (m_State == KirbyLightState::TurningOff)
    {
        float t = clamp(m_AccTime / m_TurnOffTime, 0.f, 1.f);
        t = 1.f - t;
        float NewRadius = m_MinFallOfEnd + (m_MaxFallOfEnd - m_MinFallOfEnd) * t;

        GetOwner()->Light()->SetFallOffEnd(NewRadius);
        GetOwner()->Light()->SetFallOffStart(NewRadius * m_FallRatio);
        GetOwner()->Light()->SetLightRadiance(Vec3(500.f, 50.f, 50.f) / 255.f);

        if (m_AccTime > m_TurnOffTime)
        {
            if (m_State == KirbyLightState::Drop)
            {
                GetOwner()->SetActive(false);
            }

            m_State = KirbyLightState::NONE;
        }
    }
    else if (m_State == KirbyLightState::Drop)
    {
        float t = clamp(m_AccTime / m_TurnOffTime, 0.f, 1.f);
        t = 1.f - t;
        float NewRadius = m_MaxFallOfEnd * t;

        GetOwner()->Light()->SetFallOffEnd(NewRadius);
        GetOwner()->Light()->SetFallOffStart(NewRadius * m_FallRatio);
        GetOwner()->Light()->SetLightRadiance(Vec3(1500.f, 100.f, 100.f) / 255.f);

        if (m_AccTime > m_TurnOffTime)
        {
            m_State = KirbyLightState::NONE;
            GetOwner()->SetActive(false);
        }
    }
}

void CKirbyLightScript::Init()
{
    GetOwner()->SetActive(true);
    GetOwner()->Light()->SetFallOffEnd(m_MinFallOfEnd);
    GetOwner()->Light()->SetFallOffStart(m_MinFallOfEnd * m_FallRatio);
    GetOwner()->Light()->SetLightRadiance(Vec3(500.f, 50.f, 50.f) / 255.f);
}

void CKirbyLightScript::Drop()
{
    float CurRatio = 1.f - GetOwner()->Light()->GetFallOffEnd() / m_MaxFallOfEnd;
    m_AccTime = m_TurnOffTime * CurRatio;
    m_State = KirbyLightState::Drop;
}

void CKirbyLightScript::TurnOn()
{
    if (m_State == KirbyLightState::NONE)
    {
        m_AccTime = 0.f;
    }
    else
    {
        float CurRatio = (GetOwner()->Light()->GetFallOffEnd() - m_MinFallOfEnd) / (m_MaxFallOfEnd - m_MinFallOfEnd);
        m_AccTime = m_TurnOnTime * CurRatio;
    }

    m_State = KirbyLightState::TurningOn;
}

void CKirbyLightScript::TurnOff()
{
    if (m_State == KirbyLightState::NONE)
    {
        m_AccTime = 0.f;
    }
    else
    {
        float CurRatio = 1.f - (GetOwner()->Light()->GetFallOffEnd() - m_MinFallOfEnd) / (m_MaxFallOfEnd - m_MinFallOfEnd);
        m_AccTime = m_TurnOffTime * CurRatio;
    }

    m_State = KirbyLightState::TurningOff;
}

UINT CKirbyLightScript::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CKirbyLightScript::LoadFromLevelFile(FILE* _File)
{
    return 0;
}