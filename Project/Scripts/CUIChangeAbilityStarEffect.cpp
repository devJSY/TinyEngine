#include "pch.h"
#include "CUIChangeAbilityStarEffect.h"

#include "CUIChangeAbilityStopStarEffect.h"

CUIChangeAbilityStarEffect::CUIChangeAbilityStarEffect()
    : CScript(UICHANGEABILITYSTAREFFECT)
    , m_vCenterPos{}
    , m_vOriginScale{}
    , m_eState(UIChangeAbilityStarState::GrowUp)
    , m_fSpawnStarDeleteTime(0.f)
    , m_fOriginRadius(0.f)
    , m_fRadiusSpeed(0.f)
    , m_fTheta(0.f)
    , m_fOriginTheta(0.f)
    , m_fThetaSpeed(0.f)
    , m_fMaxThreshHold(0.f)
    , m_fStarSpawnTime(0.f)
    , m_fAccTime(0.f)
    , m_fBeginTheta(0.f)
    , m_bLittleStar(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fBeginTheta, "Begin Theta", 0.01f);
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vCenterPos, "CenterPos", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fOriginRadius, "Origin Radius", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRadiusSpeed, "Grow Radius Speed", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTheta, "Theta", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fThetaSpeed, "Grow Theta Speed", 1.f);
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bLittleStar, "Is Little Star");
}

CUIChangeAbilityStarEffect::CUIChangeAbilityStarEffect(const CUIChangeAbilityStarEffect& Origin)
    : CScript(Origin)
    , m_vCenterPos{}
    , m_vOriginScale{}
    , m_eState(UIChangeAbilityStarState::GrowUp)
    , m_fSpawnStarDeleteTime(Origin.m_fSpawnStarDeleteTime)
    , m_fOriginRadius(m_fOriginRadius)
    , m_fRadiusSpeed(Origin.m_fRadiusSpeed)
    , m_fTheta(0.f)
    , m_fOriginTheta(0.f)
    , m_fThetaSpeed(Origin.m_fThetaSpeed)
    , m_fMaxThreshHold(Origin.m_fMaxThreshHold)
    , m_fStarSpawnTime(Origin.m_fStarSpawnTime)
    , m_fAccTime(0.f)
    , m_fBeginTheta(Origin.m_fBeginTheta)
    , m_bLittleStar(Origin.m_bLittleStar)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fBeginTheta, "Begin Theta", 0.01f);
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vCenterPos, "CenterPos", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fOriginRadius, "Origin Radius", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRadiusSpeed, "Grow Radius Speed", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTheta, "Theta", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fThetaSpeed, "Theta Speed", 1.f);
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bLittleStar, "Is Little Star");
}

CUIChangeAbilityStarEffect::~CUIChangeAbilityStarEffect()
{
}

void CUIChangeAbilityStarEffect::InitializeSetting(const Vec3 _vCenterPos, const float _fOriginRadius, const float _fRadiusSpeed, const float _fTheta,
                                                   const float _fThetaSpeed, const float _fSpawnStarDeleteTime, const float _fMaxThresholdStar,
                                                   const float _fStarSpawnTime, const float _fBeginTheta)
{
    m_vCenterPos = _vCenterPos;
    m_fOriginRadius = _fOriginRadius;
    m_fRadiusSpeed = _fRadiusSpeed;
    m_fTheta = _fTheta;
    m_fThetaSpeed = _fThetaSpeed;
    m_fSpawnStarDeleteTime = _fSpawnStarDeleteTime;
    m_fMaxThreshHold = _fMaxThresholdStar;
    m_fStarSpawnTime = _fStarSpawnTime;
    m_fBeginTheta = _fBeginTheta;
}

void CUIChangeAbilityStarEffect::begin()
{
    CTransform* pTr = Transform();
    if (nullptr != pTr)
        m_vOriginScale = pTr->GetWorldScale();

    m_fTheta = XMConvertToRadians(m_fTheta + m_fBeginTheta);
    m_fOriginTheta = m_fTheta;
}

void CUIChangeAbilityStarEffect::tick()
{
    switch (m_eState)
    {
    case UIChangeAbilityStarState::GrowUp: {
        SpawnStar();
        GrowUp();
    }
    break;
    case UIChangeAbilityStarState::GrowDown: {
        SpawnStar();
        GrowDown();
    }
    break;
    case UIChangeAbilityStarState::Destroy: {
        Destroy();
    }
    break;
    default:
        break;
    }
}

void CUIChangeAbilityStarEffect::GrowUp()
{
    CTransform* pTr = Transform();
    if (nullptr != pTr)
    {
        Vec3 vPos = pTr->GetWorldPos();

        m_fTheta += DT_ENGINE * m_fThetaSpeed;

        m_fOriginRadius += DT_ENGINE * m_fRadiusSpeed;

        vPos.x = m_vCenterPos.x + m_fOriginRadius * cosf(m_fTheta);
        vPos.y = m_vCenterPos.y + m_fOriginRadius * sinf(m_fTheta);

        pTr->SetWorldPos(vPos);

        float fRatio = (m_fTheta - m_fOriginTheta) / XM_PIDIV2;

        if (m_fTheta - m_fOriginTheta >= XM_PIDIV2)
        {
            fRatio = 1.f;
            ChangeState(UIChangeAbilityStarState::Destroy);
        }

        ScalingAndRotating(fRatio);
    }
    else
    {
        ChangeState(UIChangeAbilityStarState::Destroy);
    }
}

void CUIChangeAbilityStarEffect::GrowDown()
{
    CTransform* pTr = Transform();
    if (nullptr != pTr)
    {
        Vec3 vPos = pTr->GetWorldPos();

        m_fTheta += DT_ENGINE * m_fThetaSpeed;

        m_fOriginRadius += DT_ENGINE * m_fRadiusSpeed;

        vPos.x = m_vCenterPos.x + m_fOriginRadius * cosf(m_fTheta);
        vPos.y = m_vCenterPos.y + m_fOriginRadius * sinf(m_fTheta);

        pTr->SetWorldPos(vPos);

        float fRatio = (m_fTheta - m_fOriginTheta) / XM_PI;

        if (m_fTheta - m_fOriginTheta >= XM_PI)
        {
            fRatio = 1.f;
            ChangeState(UIChangeAbilityStarState::Destroy);
        }

        ScalingAndRotating((1.f - fRatio));
    }
}

void CUIChangeAbilityStarEffect::Destroy()
{
    GamePlayStatic::DestroyGameObject(GetOwner());
}

void CUIChangeAbilityStarEffect::SpawnStar()
{
    m_fAccTime += DT_ENGINE;
    if (m_fStarSpawnTime <= m_fAccTime)
    {
        m_fAccTime = 0.f;
        CGameObject* pStarEffect = nullptr;

        if (m_bLittleStar)
        {
            pStarEffect =
                CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_AbilityStopStar2.pref", L"prefab\\Effect_AbilityStopStar2.pref")->Instantiate();
        }
        else
        {
            pStarEffect =
                CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_AbilityStopStar.pref", L"prefab\\Effect_AbilityStopStar.pref")->Instantiate();
        }

        if (nullptr == pStarEffect)
            return;

        pStarEffect->Transform()->SetWorldPos(Transform()->GetWorldPos());
        pStarEffect->Transform()->SetWorldScale(Transform()->GetWorldScale());

        if (nullptr != pStarEffect->GetScript<CUIChangeAbilityStopStarEffect>())
        {
            pStarEffect->GetScript<CUIChangeAbilityStopStarEffect>()->SetDeleteTime(m_fSpawnStarDeleteTime);
            pStarEffect->GetScript<CUIChangeAbilityStopStarEffect>()->SetTheta(m_fTheta);
        }

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());
    }
}

void CUIChangeAbilityStarEffect::ScalingAndRotating(const float _fRatio)
{
    CTransform* pTr = Transform();
    if (nullptr != pTr)
    {
        Vec3 vScale = m_vOriginScale;
        vScale.x = _fRatio * m_fMaxThreshHold + m_vOriginScale.x;
        vScale.y = _fRatio * m_fMaxThreshHold + m_vOriginScale.y;

        pTr->SetWorldScale(vScale);
        pTr->SetWorldRotation(Vec3(0.f, 0.f, m_fTheta));
    }
}

UINT CUIChangeAbilityStarEffect::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fOriginRadius, sizeof(float), 1, _File);
    fwrite(&m_fRadiusSpeed, sizeof(float), 1, _File);
    fwrite(&m_fTheta, sizeof(float), 1, _File);
    fwrite(&m_fThetaSpeed, sizeof(float), 1, _File);
    fwrite(&m_fBeginTheta, sizeof(float), 1, _File);
    fwrite(&m_bLittleStar, sizeof(bool), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CUIChangeAbilityStarEffect::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fOriginRadius, sizeof(float), 1, _File);
    fread(&m_fRadiusSpeed, sizeof(float), 1, _File);
    fread(&m_fTheta, sizeof(float), 1, _File);
    fread(&m_fThetaSpeed, sizeof(float), 1, _File);
    fread(&m_fBeginTheta, sizeof(float), 1, _File);
    fread(&m_bLittleStar, sizeof(bool), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}