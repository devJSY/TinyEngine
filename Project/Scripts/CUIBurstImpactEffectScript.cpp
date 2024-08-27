#include "pch.h"
#include "CUIBurstImpactEffectScript.h"

CUIBurstImpactEffectScript::CUIBurstImpactEffectScript()
    : CScript(UIBURSTIMPACTEFFECTSCRIPT)
    , m_eState(CUIBurstImpactEffectState::Enter)
    , m_fScale(60.f)
{
}

CUIBurstImpactEffectScript::CUIBurstImpactEffectScript(const CUIBurstImpactEffectScript& Origin)
    : CScript(Origin)
    , m_eState(CUIBurstImpactEffectState::Enter)
    , m_fScale(60.f)
{
}

CUIBurstImpactEffectScript::~CUIBurstImpactEffectScript()
{
}

void CUIBurstImpactEffectScript::tick()
{
    switch (m_eState)
    {
    case CUIBurstImpactEffectState::Enter: {
        Enter();
    }
    break;
    case CUIBurstImpactEffectState::End: {
        End();
    }
    break;
    default:
        break;
    }
}

void CUIBurstImpactEffectScript::Enter()
{
    CTransform* pTr = Transform();
    if (nullptr != pTr)
    {
        m_fScale = Lerp(m_fScale, 1.f, DT * 3.f);
        pTr->SetWorldScale(Vec3(1.f, 1.f, 1.f) * m_fScale);
    }
}

void CUIBurstImpactEffectScript::End()
{
    GamePlayStatic::DestroyGameObject(GetOwner());
}

UINT CUIBurstImpactEffectScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CUIBurstImpactEffectScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}