#include "pch.h"
#include "CMomentaryObjScript.h"

CMomentaryObjScript::CMomentaryObjScript()
    : CScript(MOMENTARYOBJSCRIPT)
    , m_AccTime(0.f)
    , m_LifeTime(5.f)
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
    m_AccTime += DT;

    if (Animator() && !m_EndAnim.empty() && (m_LifeTime - m_AccTime) <= m_EndAnimPlayTime)
    {
        GetOwner()->Animator()->Play(m_EndAnim, false);
        m_EndAnim = L"";
    }

    if (m_AccTime > m_LifeTime)
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
        if (m_LifeTime > Time)
        {
            m_EndAnimPlayTime = Time;
            m_EndAnim = _Key;
        }
    }
}

UINT CMomentaryObjScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CMomentaryObjScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}
