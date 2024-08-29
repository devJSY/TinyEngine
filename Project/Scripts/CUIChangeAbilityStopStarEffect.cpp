#include "pch.h"
#include "CUIChangeAbilityStopStarEffect.h"

CUIChangeAbilityStopStarEffect::CUIChangeAbilityStopStarEffect()
    : CScript(UICHANGEABILITYSTOPSTAREFFECT)
    , m_vOriginScale{}
    , m_fAccTime(0.f)
    , m_fDeleteTime(0.f)
{
}

CUIChangeAbilityStopStarEffect::CUIChangeAbilityStopStarEffect(const CUIChangeAbilityStopStarEffect& Origin)
    : CScript(Origin)
    , m_vOriginScale{}
    , m_fAccTime(0.f)
    , m_fDeleteTime(Origin.m_fDeleteTime)
{
}

CUIChangeAbilityStopStarEffect::~CUIChangeAbilityStopStarEffect()
{
}

void CUIChangeAbilityStopStarEffect::begin()
{
    CTransform* pTr = Transform();
    if (nullptr != pTr)
    {
        m_vOriginScale = pTr->GetWorldScale();
    }
}

void CUIChangeAbilityStopStarEffect::tick()
{
    switch (m_eState)
    {
    case CUIChangeAbilityStopStarEffectState::Progress: {
        Progress();
    }
    break;
    case CUIChangeAbilityStopStarEffectState::End: {
        End();
    }
    break;
    default:
        break;
    }
}

void CUIChangeAbilityStopStarEffect::Progress()
{
    m_fAccTime += DT_ENGINE;

    float fRatio = m_fAccTime / m_fDeleteTime;
    if (m_fDeleteTime <= m_fAccTime)
    {
        fRatio = 1.f;
        ChangeState(CUIChangeAbilityStopStarEffectState::End);
    }

    ScalingAndRotating(fRatio);
}

void CUIChangeAbilityStopStarEffect::End()
{
    GamePlayStatic::DestroyGameObject(GetOwner());
}

void CUIChangeAbilityStopStarEffect::ScalingAndRotating(const float _fRatio)
{
    CTransform* pTr = Transform();
    if (nullptr != pTr)
    {
        Vec3 vScale = m_vOriginScale;
        vScale.x = vScale.x * (1.f - _fRatio);
        vScale.y = vScale.y * (1.f - _fRatio);

        pTr->SetWorldScale(vScale);
        pTr->SetWorldRotation(Vec3(0.f, 0.f, m_fTheta));
    }
}

UINT CUIChangeAbilityStopStarEffect::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CUIChangeAbilityStopStarEffect::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}