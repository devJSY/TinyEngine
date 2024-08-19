#include "pch.h"
#include "CEnterUIScript.h"

CEnterUIScript::CEnterUIScript()
    : CScript(ENTERUISCRIPT)
    , m_eState(EnterUIState::None)
    , m_pChildObj{}
    , m_bIsFinish(false)
    , m_fFadeInTime(0.f)
    , m_fFadeOutTime(0.f)
    , m_fAccTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fFadeInTime, "FadeInTime");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fFadeOutTime, "FadeOutTime");
}

CEnterUIScript::CEnterUIScript(const CEnterUIScript& Origin)
    : CScript(Origin)
    , m_eState(EnterUIState::None)
    , m_pChildObj{}
    , m_bIsFinish(false)
    , m_fFadeInTime(Origin.m_fFadeInTime)
    , m_fFadeOutTime(Origin.m_fFadeOutTime)
    , m_fAccTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fFadeInTime, "FadeInTime");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fFadeOutTime, "FadeOutTime");
}

CEnterUIScript::~CEnterUIScript()
{
}

void CEnterUIScript::begin()
{
    m_bIsFinish = false;
    m_fAccTime = 0.f;

    m_pChildObj = GetOwner()->GetChildObject();

    for (size_t i = 0; i < m_pChildObj.size(); i++)
    {
        if (nullptr != m_pChildObj[i])
        {
            m_pChildObj[i]->MeshRender()->GetMaterial(0)->SetScalarParam(INT_3, 1);
            m_pChildObj[i]->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, 0.f);
        }
    }
}

void CEnterUIScript::tick()
{
    switch (m_eState)
    {
    case EnterUIState::None:
        break;
    case EnterUIState::Enter: {
        Enter();
    }
    break;
    case EnterUIState::FadeOut: {
        FadeOut();
    }
    break;
    case EnterUIState::End:
        break;
    default:
        break;
    }
}

void CEnterUIScript::EnterState()
{
    switch (m_eState)
    {
    case EnterUIState::None:
        break;
    case EnterUIState::Enter: {
        m_fAccTime = 0.f;
    }
    break;
    case EnterUIState::FadeOut: {
        m_fAccTime = 0.f;
    }
    break;
    case EnterUIState::End: {
        m_fAccTime = 0.f;

        m_bIsFinish = true;

        if (nullptr != m_pChildObj[0])
            m_pChildObj[0]->GetParent()->SetActive(false);
    }
    break;
    default:
        break;
    }
}

void CEnterUIScript::ChangeState(EnterUIState _eState)
{
    ExitState();
    m_eState = _eState;
    EnterState();
}

void CEnterUIScript::ExitState()
{
    switch (m_eState)
    {
    case EnterUIState::None:
        break;
    case EnterUIState::Enter:
        break;
    case EnterUIState::FadeOut: {
        m_fAccTime = 0.f;
    }
    break;
    case EnterUIState::End:
        break;
    default:
        break;
    }
}

void CEnterUIScript::Enter()
{
    m_fAccTime += DT;

    float fRatio = m_fAccTime / m_fFadeInTime;

    for (size_t i = 0; i < m_pChildObj.size(); i++)
    {
        if (nullptr != m_pChildObj[i])
        {
            m_pChildObj[i]->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, fRatio);
        }
    }

    if (m_fFadeInTime <= m_fAccTime)
    {
        ChangeState(EnterUIState::FadeOut);
    }
}

void CEnterUIScript::FadeOut()
{
    m_fAccTime += DT;

    if (m_fFadeOutTime <= m_fAccTime)
    {
        ChangeState(EnterUIState::End);
        m_fAccTime = m_fFadeOutTime;
    }

    float fRatio = m_fAccTime / m_fFadeOutTime;

    for (size_t i = 0; i < m_pChildObj.size(); i++)
    {
        if (nullptr != m_pChildObj[i])
        {
            m_pChildObj[i]->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, (1.f - fRatio));
        }
    }
}

UINT CEnterUIScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fFadeInTime, sizeof(float), 1, _File);
    fwrite(&m_fFadeOutTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CEnterUIScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fFadeInTime, sizeof(float), 1, _File);
    fread(&m_fFadeOutTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}