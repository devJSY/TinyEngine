#include "pch.h"
#include "CChangeAlphaScript.h"

CChangeAlphaScript::CChangeAlphaScript()
    : CScript(CHANGEALPHASCRIPT)
    , m_Event(ChangeAlphaEvent::NONE)
    , m_PlayTime(0.f)
    , m_AccTime(0.f)

{
}

CChangeAlphaScript::~CChangeAlphaScript()
{
}

void CChangeAlphaScript::begin()
{
    for (int i = 0; i < MeshRender()->GetMtrlCount(); ++i)
    {
        m_listMtrl.push_back(MeshRender()->GetMaterial(i));
    }
}

void CChangeAlphaScript::tick()
{
    m_AccTime += DT;

    switch (m_Event)
    {
    case ChangeAlphaEvent::FADE_IN: {
        float Alpha = clamp(m_AccTime / m_PlayTime, 0.f, 1.f);
        SetAlpha(Alpha);

        if (m_AccTime > m_PlayTime)
        {
            ClearEvent();
        }
    }
    break;
    case ChangeAlphaEvent::FADE_OUT: {
        float Alpha = 1.f - clamp(m_AccTime / m_PlayTime, 0.f, 1.f);
        SetAlpha(Alpha);

        if (m_AccTime > m_PlayTime)
        {
            GamePlayStatic::DestroyGameObject(GetOwner());
        }
    }
    break;
    }
}

void CChangeAlphaScript::SetAlpha(float _Alpha)
{
    for (Ptr<CMaterial> iter : m_listMtrl)
    {
        iter->SetScalarParam(SCALAR_PARAM::FLOAT_2, _Alpha);
    }
}

void CChangeAlphaScript::SetEvent(ChangeAlphaEvent _Type, float _PlayTime)
{
    m_Event = _Type;
    m_AccTime = 0.f;
    m_PlayTime = _PlayTime;
}

void CChangeAlphaScript::ClearEvent()
{
    m_Event = ChangeAlphaEvent::NONE;
    m_AccTime = 0.f;
    m_PlayTime = 0.f;
}

UINT CChangeAlphaScript::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CChangeAlphaScript::LoadFromLevelFile(FILE* _File)
{
    return 0;
}
