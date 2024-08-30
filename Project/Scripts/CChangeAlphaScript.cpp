#include "pch.h"
#include "CChangeAlphaScript.h"

CChangeAlphaScript::CChangeAlphaScript()
    : CScript(CHANGEALPHASCRIPT)
    , m_AlphaParamIdx(SCALAR_PARAM::FLOAT_2)
    , m_Event(ChangeAlphaEvent::NONE)
    , m_PlayTime(0.f)
    , m_MinPlayTime(0.f)
    , m_AccTime(0.f)
{
}

CChangeAlphaScript::~CChangeAlphaScript()
{
}

void CChangeAlphaScript::begin()
{
    // get mtrl
    deque<CGameObject*> Queue;
    Queue.push_back(GetOwner());

    while (!Queue.empty())
    {
        CGameObject* iter = Queue.front();
        Queue.pop_front();

        vector<CGameObject*> vecChild = iter->GetChildObject();
        for (CGameObject* iter2 : vecChild)
        {
            Queue.push_back(iter2);
        }

        if (iter->MeshRender())
        {
            for (int i = 0; i < (int)iter->MeshRender()->GetMtrlCount(); ++i)
            {
                m_listMtrl.push_back(iter->MeshRender()->GetMaterial(i));
            }
        }
    }
}

void CChangeAlphaScript::tick()
{
    m_AccTime += DT;

    switch (m_Event)
    {
    case ChangeAlphaEvent::FadeIn: {
        float Alpha = clamp(m_AccTime / m_PlayTime, 0.f, 1.f);
        SetAlpha(Alpha);

        if (m_AccTime > m_PlayTime)
        {
            ClearEvent();
        }
    }
    break;
    case ChangeAlphaEvent::FadeIn_Random: {
        float Alpha = clamp(m_AccTime / m_PlayTime, 0.f, 1.f);
        SetRandAlpha(Alpha);

        if (m_AccTime > m_PlayTime)
        {
            ClearEvent();
        }
    }
    break;
    case ChangeAlphaEvent::FadeOut: {
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
        iter->SetScalarParam(m_AlphaParamIdx, _Alpha);
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
