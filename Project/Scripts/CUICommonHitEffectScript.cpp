#include "pch.h"
#include "CUICommonHitEffectScript.h"

#include "CPlayerMgr.h"
#include "CCameraController.h"

CUICommonHitEffectScript::CUICommonHitEffectScript()
    : CScript(UICOMMONHITEFFECTSCRIPT)
    , m_vOriginScale{}
    , m_eState(CommonHitEffectState::Progress)
    , m_fLerpSpeed(0.f)
    , m_fRatio(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRatio, "Current Scale Ratio");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLerpSpeed, "Scale Lerp Speed");
}

CUICommonHitEffectScript::CUICommonHitEffectScript(const CUICommonHitEffectScript& Origin)
    : CScript(Origin)
    , m_vOriginScale{}
    , m_eState(CommonHitEffectState::Progress)
    , m_fLerpSpeed(Origin.m_fLerpSpeed)
    , m_fRatio(Origin.m_fRatio)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRatio, "Current Scale Ratio");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLerpSpeed, "Scale Lerp Speed");
}

CUICommonHitEffectScript::~CUICommonHitEffectScript()
{
}

void CUICommonHitEffectScript::begin()
{
    CTransform* pTr = Transform();
    if (nullptr != pTr)
    {
        m_vOriginScale = pTr->GetWorldScale();
    }
}

void CUICommonHitEffectScript::tick()
{
    switch (m_eState)
    {
    case CommonHitEffectState::Progress: {
        Progress();
    }
    break;
    case CommonHitEffectState::End: {
        Stop();
    }
    break;
    default:
        break;
    }
}

void CUICommonHitEffectScript::Progress()
{
    CTransform* pTr = Transform();
    if (nullptr != pTr)
    {
        m_fRatio = Lerp(m_fRatio, 1.f, m_fLerpSpeed * DT);
        Vec3 vScale = m_vOriginScale * m_fRatio;
        pTr->SetWorldScale(vScale);

        if (m_fRatio >= 0.99f)
        {
            m_eState = CommonHitEffectState::End;
        }
    }

    Billboarding();
}

void CUICommonHitEffectScript::Stop()
{
    GamePlayStatic::DestroyGameObject(GetOwner());
}

void CUICommonHitEffectScript::Billboarding()
{
    if (CAMERACTRL)
    {
        Vec3 LookDir = (CAMERACTRL->GetOwner()->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
        Transform()->SetDirection(LookDir);
    }
}

UINT CUICommonHitEffectScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fRatio, sizeof(float), 1, _File);
    fwrite(&m_fLerpSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CUICommonHitEffectScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fRatio, sizeof(float), 1, _File);
    fread(&m_fLerpSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
