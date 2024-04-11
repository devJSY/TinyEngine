#include "pch.h"
#include "CFXScript.h"

CFXScript::CFXScript()
    : CScript(FXSCRIPT)
    , m_Duration(0.f)
    , m_PassedTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
}

CFXScript::CFXScript(const CFXScript& origin)
    : CScript(origin)
    , m_Duration(origin.m_Duration)
    , m_PassedTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
}

CFXScript::~CFXScript()
{
}

void CFXScript::tick()
{
    m_PassedTime += DT;

    if (nullptr != Animator2D())
    {
        if (Animator2D()->IsFinish())
        {
            GamePlayStatic::DestroyGameObject(GetOwner());
        }
    }
    else
    {
        if (m_PassedTime > m_Duration)
        {
            GamePlayStatic::DestroyGameObject(GetOwner());
        }
    }
}

void CFXScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Duration, sizeof(float), 1, _File);
}

void CFXScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Duration, sizeof(float), 1, _File);
}
