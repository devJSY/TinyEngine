#include "pch.h"
#include "CUIChangeAbilityStarEffect.h"

CUIChangeAbilityStarEffect::CUIChangeAbilityStarEffect()
    : CScript(UICHANGEABILITYSTAREFFECT)
    , m_vCenterPos{}
    , m_eState(UIChangeAbilityStarState::Move)
    , m_fOriginRadius(0.f)
    , m_fRadiusSpeeed(0.f)
    , m_fTheta(0.f)
    , m_fOriginTheta(0.f)
    , m_fThetaSpeed(0.f)
    , m_fPrevTheta(0.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vCenterPos, "CenterPos", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fOriginRadius, "Origin Radius", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRadiusSpeeed, "Progress Radius", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTheta, "Theta", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fThetaSpeed, "Theta Speed", 1.f);
}

CUIChangeAbilityStarEffect::CUIChangeAbilityStarEffect(const CUIChangeAbilityStarEffect& Origin)
    : CScript(Origin)
    , m_vCenterPos{}
    , m_eState(UIChangeAbilityStarState::Move)
    , m_fOriginRadius(m_fOriginRadius)
    , m_fRadiusSpeeed(Origin.m_fRadiusSpeeed)
    , m_fTheta(0.f)
    , m_fOriginTheta(0.f)
    , m_fThetaSpeed(Origin.m_fThetaSpeed)
    , m_fPrevTheta(0.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vCenterPos, "CenterPos", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fOriginRadius, "Origin Radius", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRadiusSpeeed, "Progress Radius", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTheta, "Theta", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fThetaSpeed, "Theta Speed", 1.f);
}

CUIChangeAbilityStarEffect::~CUIChangeAbilityStarEffect()
{
}

void CUIChangeAbilityStarEffect::begin()
{
    m_fPrevTheta = m_fTheta;
    m_fOriginTheta = m_fTheta;
}

void CUIChangeAbilityStarEffect::tick()
{
    switch (m_eState)
    {
    case UIChangeAbilityStarState::Move: {
        Move();
    }
    break;
    case UIChangeAbilityStarState::Stop: {
        Stop();
    }
    break;
    default:
        break;
    }
}

void CUIChangeAbilityStarEffect::Move()
{
    CTransform* pTr = Transform();
    if (nullptr != pTr)
    {
        Vec3 vPos = pTr->GetWorldPos();

        m_fTheta += DT * m_fThetaSpeed;

        m_fOriginRadius += DT * m_fRadiusSpeeed;

        vPos.x += m_fOriginRadius * cosf(m_fTheta);
        vPos.y += m_fOriginRadius * sinf(m_fTheta);

        pTr->SetWorldPos(vPos);

        SpawnStar();

        if (m_fTheta - m_fOriginTheta >= XM_PIDIV2)
        {
            ChangeState(UIChangeAbilityStarState::Stop);
        }
    }
    else
    {
        ChangeState(UIChangeAbilityStarState::Stop);
    }
}

void CUIChangeAbilityStarEffect::Stop()
{
    // GamePlayStatic::DestroyGameObject(GetOwner());
}

void CUIChangeAbilityStarEffect::SpawnStar()
{
    if (m_fTheta - m_fPrevTheta >= 0.53f)
    {
        m_fPrevTheta = m_fTheta;

        CGameObject* pStarEffect =
            CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_AbilityStopStar.pref", L"prefab\\Effect_AbilityStopStar.pref")->Instantiate();

        pStarEffect->Transform()->SetWorldPos(Transform()->GetWorldPos());

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());
    }
}

UINT CUIChangeAbilityStarEffect::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fOriginRadius, sizeof(float), 1, _File);
    fwrite(&m_fRadiusSpeeed, sizeof(float), 1, _File);
    fwrite(&m_fTheta, sizeof(float), 1, _File);
    fwrite(&m_fThetaSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CUIChangeAbilityStarEffect::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fOriginRadius, sizeof(float), 1, _File);
    fread(&m_fRadiusSpeeed, sizeof(float), 1, _File);
    fread(&m_fTheta, sizeof(float), 1, _File);
    fread(&m_fThetaSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}