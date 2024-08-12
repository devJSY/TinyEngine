#include "pch.h"
#include "CMomentaryObjScript.h"

CMomentaryObjScript::CMomentaryObjScript()
    : CScript(MOMENTARYOBJSCRIPT)
    , m_AccTime(0.f)
    , m_LifeTime(5.f)
    , m_ScaleFactor(Vec3(1.f))
    , m_ScaleTime(0.f)
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

    // Effect
    deque<MomentaryEffectType>::iterator iter = m_EffectVec.begin();
    for (; iter != m_EffectVec.end();)
    {
        switch (*iter)
        {
        case MomentaryEffectType::Scaling: {
            float t = 0.f;

            if (m_ScaleTime > 0.f)
            {
                t = m_AccTime / m_ScaleTime;
            }
            else
            {
                t = m_AccTime / m_LifeTime;
            }

            Transform()->SetLocalScale(m_OriginScale * m_ScaleFactor * t);

            if (t >= 1.f)
            {
                iter = m_EffectVec.erase(iter);
                continue;
            }
        }
        break;
        case MomentaryEffectType::DisappearAnim: {
            if (Animator() && !m_EndAnim.empty() && (m_LifeTime - m_AccTime) <= m_EndAnimPlayTime)
            {
                GetOwner()->Animator()->Play(m_EndAnim, false);
                m_EndAnim = L"";
            }
        }
        break;
        }

        iter++;
    }

    if (m_AccTime > m_LifeTime)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CMomentaryObjScript::AddEffect(MomentaryEffectType _EffectType)
{
    m_EffectVec.push_back(_EffectType);

    if (_EffectType == MomentaryEffectType::Scaling)
    {
        m_OriginScale = Transform()->GetLocalScale();
        m_ScaleFactor = Vec3(1.f);
        m_ScaleTime = 0.f;
    }
}

void CMomentaryObjScript::SetEndAnim(wstring _Key)
{
    if (!GetOwner()->Animator())
        return;

    int idx = GetOwner()->Animator()->FindClipIndex(_Key);
    if (idx != -1)
    {
        double Time = GetOwner()->Animator()->GetClipPlayTime();
        if (m_LifeTime > Time)
        {
            m_EndAnimPlayTime = Time;
            m_EndAnim = _Key;
            AddEffect(MomentaryEffectType::DisappearAnim);
        }
    }
}

void CMomentaryObjScript::SetScaling(Vec3 _ScaleFactor, float _ScaleTime)
{
    AddEffect(MomentaryEffectType::Scaling);
    m_ScaleFactor = _ScaleFactor;
    m_ScaleTime = _ScaleTime;
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
