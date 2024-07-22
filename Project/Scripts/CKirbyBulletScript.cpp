#include "pch.h"
#include "CKirbyBulletScript.h"

CKirbyBulletScript::CKirbyBulletScript()
    : CScript(KIRBYBULLETSCRIPT)
    , m_PlayTime(5.f)
    , m_EndAnim(L"")
    , m_EndAnimPlayTime(0)
{
}

CKirbyBulletScript::~CKirbyBulletScript()
{
}

void CKirbyBulletScript::begin()
{
    if (!Rigidbody())
    {
        GetOwner()->AddComponent(new CRigidbody());
    }

    Rigidbody()->AddForce(m_InitVelocity, ForceMode::Impulse);
}

void CKirbyBulletScript::tick()
{
    m_PlayTime -= DT;

    if (m_EndAnim != L"" && m_PlayTime <= m_EndAnimPlayTime)
    {
        GetOwner()->Animator()->Play(m_EndAnim, false);
        m_EndAnim = L"";
    }

    if (m_PlayTime <= 0.f)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CKirbyBulletScript::SetEndAnim(wstring _Key)
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

UINT CKirbyBulletScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CKirbyBulletScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}
