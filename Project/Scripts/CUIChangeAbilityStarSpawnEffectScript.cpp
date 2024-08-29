#include "pch.h"
#include "CUIChangeAbilityStarSpawnEffectScript.h"

#include <Engine\CRenderMgr.h>
#include "CPlayerMgr.h"

#include "CUIChangeAbilityStarEffect.h"

CUIChangeAbilityStarSpawnEffectScript::CUIChangeAbilityStarSpawnEffectScript()
    : CScript(UICHANGEABILITYSTARSPAWNEFFECTSCRIPT)
    , m_vCenterPos{}
    , m_vBigStarScale{}
    , m_vLittleStarScale{}
    , m_eState(UIChangeAbilityStarSpawnEffectState::Enter)
    , m_fOriginRadius(0.f)
    , m_fBigStarRadiusSpeed(0.f)
    , m_fLitttleStarRadiusSpeed(0.f)
    , m_fThetaSpeed(0.f)
    , m_fSpawnStarDeleteTime(0.f)
    , m_fThreshHoldStar(0.f)
    , m_fStarSpawnTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vBigStarScale, "Big Star Scale");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vLittleStarScale, "Little Star Scale");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fOriginRadius, "Origin Radius");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fBigStarRadiusSpeed, "Big Star Grow Radius Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLitttleStarRadiusSpeed, "Little Star Radius Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fThetaSpeed, "Theta Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpawnStarDeleteTime, "Spawn Star Delete Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fThreshHoldStar, "Max Thresh Hold Star");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fStarSpawnTime, "Star Spawn Time");
}

CUIChangeAbilityStarSpawnEffectScript::CUIChangeAbilityStarSpawnEffectScript(const CUIChangeAbilityStarSpawnEffectScript& Origin)
    : CScript(Origin)
    , m_vCenterPos{}
    , m_vBigStarScale(Origin.m_vBigStarScale)
    , m_vLittleStarScale(Origin.m_vLittleStarScale)
    , m_eState(UIChangeAbilityStarSpawnEffectState::Enter)
    , m_fOriginRadius(Origin.m_fOriginRadius)
    , m_fBigStarRadiusSpeed(Origin.m_fBigStarRadiusSpeed)
    , m_fLitttleStarRadiusSpeed(Origin.m_fLitttleStarRadiusSpeed)
    , m_fThetaSpeed(Origin.m_fThetaSpeed)
    , m_fSpawnStarDeleteTime(Origin.m_fSpawnStarDeleteTime)
    , m_fThreshHoldStar(Origin.m_fThreshHoldStar)
    , m_fStarSpawnTime(Origin.m_fStarSpawnTime)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vBigStarScale, "Big Star Scale");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vLittleStarScale, "Little Star Scale");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fOriginRadius, "Origin Radius");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fBigStarRadiusSpeed, "Big Star Grow Radius Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLitttleStarRadiusSpeed, "Little Star Radius Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fThetaSpeed, "Theta Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpawnStarDeleteTime, "Spawn Star Delete Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fThreshHoldStar, "Max Thresh Hold Star");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fStarSpawnTime, "Star Spawn Time");
}

CUIChangeAbilityStarSpawnEffectScript::~CUIChangeAbilityStarSpawnEffectScript()
{
}

void CUIChangeAbilityStarSpawnEffectScript::tick()
{
    switch (m_eState)
    {
    case UIChangeAbilityStarSpawnEffectState::Enter: {
        Enter();
    }
    break;
    case UIChangeAbilityStarSpawnEffectState::Progress: {
        Progress();
    }
    break;
    case UIChangeAbilityStarSpawnEffectState::End: {
        End();
    }
    break;
    default:
        break;
    }
}

void CUIChangeAbilityStarSpawnEffectScript::Enter()
{
    // PLAYER
    if (true)
    {
        CaculateTargetPos();
        ChangeState(UIChangeAbilityStarSpawnEffectState::Progress);
    }
    else
    {
        ChangeState(UIChangeAbilityStarSpawnEffectState::End);
    }
}

void CUIChangeAbilityStarSpawnEffectScript::Progress()
{
    for (int i = 0; i < 10; i++)
    {
        wstring prefabName = {};

        Vec3 vStarScale = {};
        float fRadiusSpeed = 0.f;
        float fTheta = 36.f * i;

        if (i % 2 == 0)
        {
            prefabName = L"Effect_AbilitySpiralStar.pref";
            fRadiusSpeed = m_fBigStarRadiusSpeed;
            vStarScale = m_vBigStarScale;
        }
        else
        {
            prefabName = L"Effect_AbilitySpiralStar2.pref";
            fRadiusSpeed = m_fLitttleStarRadiusSpeed;
            vStarScale = m_vLittleStarScale;
        }

        CGameObject* pStarEffect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\" + prefabName, L"prefab\\" + prefabName)->Instantiate();

        pStarEffect->GetScript<CUIChangeAbilityStarEffect>()->SetCenterPos(m_vCenterPos);
        pStarEffect->GetScript<CUIChangeAbilityStarEffect>()->SetOriginRadius(m_fOriginRadius);
        pStarEffect->GetScript<CUIChangeAbilityStarEffect>()->SetRadiusSpeed(fRadiusSpeed);
        pStarEffect->GetScript<CUIChangeAbilityStarEffect>()->SetTheta(fTheta);
        pStarEffect->GetScript<CUIChangeAbilityStarEffect>()->SetThetaSpeed(m_fThetaSpeed);
        pStarEffect->GetScript<CUIChangeAbilityStarEffect>()->SetSpawnStarDeleteTime(m_fSpawnStarDeleteTime);
        pStarEffect->GetScript<CUIChangeAbilityStarEffect>()->SetMaxThreshHoldStar(m_fThreshHoldStar);
        pStarEffect->GetScript<CUIChangeAbilityStarEffect>()->SetStarSpawnTime(m_fStarSpawnTime);

        pStarEffect->Transform()->SetWorldScale(vStarScale);

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());
    }

    ChangeState(UIChangeAbilityStarSpawnEffectState::End);
}

void CUIChangeAbilityStarSpawnEffectScript::End()
{
    GamePlayStatic::DestroyGameObject(GetOwner());
}

void CUIChangeAbilityStarSpawnEffectScript::CaculateTargetPos()
{
    if (nullptr == PLAYER)
        return;

    Vec3 vPlayerPos = PLAYER->Transform()->GetWorldPos();

    // MainCam
    CCamera* _pCam = CRenderMgr::GetInst()->GetCamera(0);

    Matrix _VPMatrix = _pCam->GetViewMat() * _pCam->GetProjMat();

    // WolrdPos -> NDC
    Vec3 _vPlayerNDCPos = Vector3::Transform(vPlayerPos, _VPMatrix);

    // UICam
    _pCam = CRenderMgr::GetInst()->GetCamera(2);

    // NDC -> WorldPos
    Matrix _VPInverseMatrix = _pCam->GetProjInvMat() * _pCam->GetViewInvMat();

    m_vCenterPos = Vector3::Transform(_vPlayerNDCPos, _VPInverseMatrix);
}

UINT CUIChangeAbilityStarSpawnEffectScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_vBigStarScale, sizeof(Vec3), 1, _File);
    fwrite(&m_vLittleStarScale, sizeof(Vec3), 1, _File);
    fwrite(&m_fOriginRadius, sizeof(float), 1, _File);
    fwrite(&m_fBigStarRadiusSpeed, sizeof(float), 1, _File);
    fwrite(&m_fLitttleStarRadiusSpeed, sizeof(float), 1, _File);
    fwrite(&m_fThetaSpeed, sizeof(float), 1, _File);
    fwrite(&m_fSpawnStarDeleteTime, sizeof(float), 1, _File);
    fwrite(&m_fThreshHoldStar, sizeof(float), 1, _File);
    fwrite(&m_fStarSpawnTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CUIChangeAbilityStarSpawnEffectScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_vBigStarScale, sizeof(Vec3), 1, _File);
    fread(&m_vLittleStarScale, sizeof(Vec3), 1, _File);
    fread(&m_fOriginRadius, sizeof(float), 1, _File);
    fread(&m_fBigStarRadiusSpeed, sizeof(float), 1, _File);
    fread(&m_fLitttleStarRadiusSpeed, sizeof(float), 1, _File);
    fread(&m_fThetaSpeed, sizeof(float), 1, _File);
    fread(&m_fSpawnStarDeleteTime, sizeof(float), 1, _File);
    fread(&m_fThreshHoldStar, sizeof(float), 1, _File);
    fread(&m_fStarSpawnTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}