#include "pch.h"
#include "CMomentaryObjScript.h"

CMomentaryObjScript::CMomentaryObjScript()
    : CScript(MOMENTARYOBJSCRIPT)
    , m_PlayTime(5.f)
    , m_EndAnimPlayTime(0)
    , m_EndAnim(L"")
{
}

CMomentaryObjScript::~CMomentaryObjScript()
{
}

void CMomentaryObjScript::begin()
{
    if (Rigidbody())
    {
        Rigidbody()->AddForce(m_InitVelocity, ForceMode::Impulse);
    }
}

void CMomentaryObjScript::tick()
{
    m_PlayTime -= DT;

    if (Animator() && m_EndAnim != L"" && m_PlayTime <= m_EndAnimPlayTime)
    {
        GetOwner()->Animator()->Play(m_EndAnim, false);
        m_EndAnim = L"";
    }

    if (m_PlayTime <= 0.f)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CMomentaryObjScript::SetEndAnim(wstring _Key)
{
    if (!GetOwner()->Animator())
        return;

    int idx = GetOwner()->Animator()->FindClipIndex(_Key);
    if (idx != -1)
    {
        double Time = GetOwner()->GetRenderComponent()->GetMesh()->GetAnimClip()->at(idx).dTimeLength;
        if (m_PlayTime > Time)
        {
            m_EndAnimPlayTime = Time;
            m_EndAnim = _Key;
        }
    }
}