#include "pch.h"
#include "CKirbyLightScript.h"

CKirbyLightScript::CKirbyLightScript()
    : CScript(KIRBYLIGHTSCRIPT)
    , m_OriginFallOfEnd(0.f)
    , m_TurningTime(1.f)
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

    m_OriginFallOfEnd = GetOwner()->Light()->GetFallOffEnd();
    GetOwner()->SetActive(false);
}

void CKirbyLightScript::tick()
{
    if (m_State == KirbyLightState::NONE)
        return;

    m_AccTime += DT;
    float t = clamp(m_AccTime / m_TurningTime, 0.f, 1.f);

    if (m_State == KirbyLightState::TurningOn)
    {
        GetOwner()->Light()->SetFallOffEnd(m_OriginFallOfEnd * t);

        if (m_AccTime > m_TurningTime)
        {
            m_State = KirbyLightState::NONE;
        }
    }
    else if (m_State == KirbyLightState::TurningOff)
    {
        t = 1.f - t;
        GetOwner()->Light()->SetFallOffEnd(m_OriginFallOfEnd * t);

        if (m_AccTime > m_TurningTime)
        {
            m_State = KirbyLightState::NONE;
            GetOwner()->SetActive(false);
        }
    }
}

void CKirbyLightScript::TurnOn()
{
    if (m_OriginFallOfEnd <= 0.f)
    {
        m_OriginFallOfEnd = GetOwner()->Light()->GetFallOffEnd();
    }

    if (m_State == KirbyLightState::NONE)
    {
        GetOwner()->SetActive(true);
        GetOwner()->Light()->SetFallOffEnd(0.f);
        m_AccTime = 0.f;
    }

    m_State = KirbyLightState::TurningOn;
}

void CKirbyLightScript::TurnOff()
{
    if (m_State == KirbyLightState::NONE)
    {
        m_AccTime = 0.f;
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