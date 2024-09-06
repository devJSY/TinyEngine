#include "pch.h"
#include "CFlowMgr_Lv0.h"

CFlowMgr_Lv0::CFlowMgr_Lv0()
    : CLevelFlowMgr(FLOWMGR_LV0)
    , m_PostEffectMtrl(nullptr)
    , m_LightObj(nullptr)
    , m_FogCastMode(0)
    , m_LightCastMode(0)
{
}

CFlowMgr_Lv0::CFlowMgr_Lv0(const CFlowMgr_Lv0& _Origin)
    : CLevelFlowMgr(_Origin)
    , m_PostEffectMtrl(nullptr)
    , m_LightObj(nullptr)
    , m_FogCastMode(0)
    , m_LightCastMode(0)
{
}

CFlowMgr_Lv0::~CFlowMgr_Lv0()
{
}

void CFlowMgr_Lv0::begin()
{
    CLevelFlowMgr::begin();

    SetStartLevel(true);

    LevelStart();

    SetEnterTime(6.8f);

    SetFadeEffectColor(Vec3(115.f, 156.f, 28.f));

    // Fog Setting
    m_PostEffectMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PostEffectMtrl");
    m_PostEffectMtrl->SetScalarParam(FLOAT_0, 250.f);                    // Fog Min
    m_PostEffectMtrl->SetScalarParam(FLOAT_1, 700.f);                    // Fog Max
    m_PostEffectMtrl->SetScalarParam(FLOAT_2, 0.f);                      // Fog Strength
    m_PostEffectMtrl->SetScalarParam(VEC4_0, Vec4(0.3f, 1.f, 1.f, 1.f)); // Fog Color
    SetToneMappingParam(false);                                          // Bloom Off

    m_LightObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Dynamic Directional Light");
    if (nullptr != m_LightObj)
    {
        m_LightObj->Light()->SetLightRadiance(Vec3(0.5019f, 0.5019f, 0.5019f)); // 128, 128, 128
    }

    CGameObject* pLensFlareObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"LensFlare");
    if (nullptr != pLensFlareObj)
    {
        pLensFlareObj->SetActive(false);
    }
}

void CFlowMgr_Lv0::tick()
{
    CLevelFlowMgr::tick();

    // 레벨 종료 중
    if (IsChangedLevel())
        return;

    // Fog Cast
    if (0 != m_FogCastMode)
    {
        float FogStrength = *(float*)m_PostEffectMtrl->GetScalarParam(FLOAT_2);
        FogStrength = Lerp(FogStrength, 1 == m_FogCastMode ? 0.3f : 0.f, DT);
        m_PostEffectMtrl->SetScalarParam(FLOAT_2, FogStrength);

        float ThresholdValue = 1 == m_FogCastMode ? fabs(FogStrength - 0.3f) : FogStrength;
        // FogStrength가 임계점에 도달한 경우 Cast 해제
        if (ThresholdValue < 1e-3f)
        {
            m_FogCastMode = 0;
        }
    }

    if (0 != m_LightCastMode && nullptr != m_LightObj)
    {
        Vec3 LightRadiance = m_LightObj->Light()->GetLightRadiance();
        // 500, 500, 500
        LightRadiance = Vec3::Lerp(LightRadiance, 1 == m_LightCastMode ? Vec3(1.96f, 1.96f, 1.96f) : Vec3(0.5019f, 0.5019f, 0.5019f), DT);
        m_LightObj->Light()->SetLightRadiance(LightRadiance);

        float ThresholdValue = 1 == m_LightCastMode ? Vec3(1.96f, 1.96f, 1.96f).Length() : Vec3(0.5019f, 0.5019f, 0.5019f).Length();
        // FogStrength가 임계점에 도달한 경우 Cast 해제
        if (fabs(LightRadiance.Length() - ThresholdValue) < 1e-3f)
        {
            m_LightCastMode = 0;
        }
    }
}

void CFlowMgr_Lv0::TriggerEvent(UINT _Idx)
{
    switch (_Idx)
    {
    // Post Effect Enter Trigger
    case 1: {
        m_FogCastMode = 1;
        FadeInBGM(L"sound\\wav\\EnvTownForest\\0000.wav", 0.f, SOUND_BGM, 1.f);
    }
    //  Post Effect Exit Trigger
    break;
    case 2: {
        m_FogCastMode = 2;
        FadeOutBGM(1.f);
    }
    break;
    // ToneMapping Enter Trigger
    case 3: {
        m_LightCastMode = 1;
        SetToneMappingParam(true, true, 1.2f, 0.8f);
        CGameObject* pLensFlareObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"LensFlare");
        if (nullptr != pLensFlareObj)
        {
            pLensFlareObj->SetActive(true);
        }
        FadeInBGM(L"sound\\stream\\K15_Grassland1\\K15_Grassland1.marker.wav", 0.f, SOUND_BGM, 1.f);
    }
    break;
    // ToneMapping Exit Trigger
    case 4: {
        m_LightCastMode = 2;
        SetToneMappingParam(false);
        CGameObject* pLensFlareObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"LensFlare");
        if (nullptr != pLensFlareObj)
        {
            pLensFlareObj->SetActive(false);
        }
    }
    break;
    // LensFlare Enter Trigger
    case 5: {
        CGameObject* pLensFlareObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"LensFlare");
        if (nullptr != pLensFlareObj)
        {
            pLensFlareObj->SetActive(true);
        }
    }
    break;
    // LensFlare Exit Trigger
    case 6: {
        CGameObject* pLensFlareObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"LensFlare");
        if (nullptr != pLensFlareObj)
        {
            pLensFlareObj->SetActive(false);
        }
    }
    break;
    default:
        break;
    }
}

void CFlowMgr_Lv0::LevelEnd()
{
    CLevelFlowMgr::LevelEnd();

    SetFadeEffectColor(Vec3(115.f, 156.f, 28.f));
}

void CFlowMgr_Lv0::LevelExit()
{
    CLevelFlowMgr::LevelExit();

    m_PostEffectMtrl->SetScalarParam(FLOAT_2, 0.f); // Fog Strength
    SetToneMappingParam(false);                     // Bloom Off
}

void CFlowMgr_Lv0::LevelRestart()
{
    CLevelFlowMgr::LevelRestart();
    SetFadeEffectColor(Vec3(115.f, 156.f, 28.f));
}

UINT CFlowMgr_Lv0::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CFlowMgr_Lv0::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}
