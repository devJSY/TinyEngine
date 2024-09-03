#include "pch.h"
#include "CUISlashEffectScript.h"

#include "CPlayerMgr.h"
#include "CCameraController.h"

CUISlashEffectScript::CUISlashEffectScript()
    : CScript(UISLASHEFFECTSCRIPT)
    , m_vOriginScale{}
    , m_eState(UISlashEffectState::Progress)
    , m_fRatio(3.f)
    , m_fLerpSpeed(0.f)
    , m_fLessRatio(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLerpSpeed, "Lerp Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLessRatio, "Less Ratio");
}

CUISlashEffectScript::CUISlashEffectScript(const CUISlashEffectScript& Origin)
    : CScript(Origin)
    , m_vOriginScale{}
    , m_eState(UISlashEffectState::Progress)
    , m_fRatio(Origin.m_fLerpSpeed)
    , m_fLerpSpeed(Origin.m_fLerpSpeed)
    , m_fLessRatio(Origin.m_fLessRatio)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLerpSpeed, "Lerp Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLessRatio, "Less Ratio");
}

CUISlashEffectScript::~CUISlashEffectScript()
{
}

void CUISlashEffectScript::begin()
{
    CTransform* pTr = Transform();
    CMeshRender* pRender = MeshRender();
    if (pTr)
    {
        m_fRatio = 15.f;
        m_vOriginScale = Vec3(m_fRatio, 100.f, 1.f);
        pTr->SetWorldScale(m_vOriginScale);
        Billboarding();
        Vec3 vRot = pTr->GetWorldRotation();
        vRot.z += RandomRotation();
        pTr->SetWorldRotation(vRot);

        pRender->GetMaterial(0)->SetAlbedo(Vec4(3.f, 3.f, 0.f, 1.f));
    }
}

void CUISlashEffectScript::tick()
{
    switch (m_eState)
    {
    case UISlashEffectState::Progress: {
        Progress();
    }
    break;
    case UISlashEffectState::End: {
        Stop();
    }
    break;
    default:
        break;
    }
}

void CUISlashEffectScript::Billboarding()
{
    if (CAMERACTRL)
    {
        Vec3 LookDir = (CAMERACTRL->GetOwner()->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
        Transform()->SetDirection(LookDir);
    }
}

float CUISlashEffectScript::RandomRotation()
{
    return GetRandomfloat(0.f, XM_PI);
}

void CUISlashEffectScript::Progress()
{
    CTransform* pTr = Transform();
    if (nullptr != pTr)
    {
        m_fRatio = Lerp(m_fRatio, 1.f, DT * 10.f);
        Vec3 vScale = m_vOriginScale;
        vScale.x = m_fRatio;

        if (m_fRatio <= 1.1f)
        {
            m_eState = UISlashEffectState::End;
            vScale.x = 1.f;
        }

        pTr->SetWorldScale(vScale);
    }
}

void CUISlashEffectScript::Stop()
{
    GamePlayStatic::DestroyGameObject(GetOwner());
}

UINT CUISlashEffectScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fLerpSpeed, sizeof(float), 1, _File);
    fwrite(&m_fLessRatio, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CUISlashEffectScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fLerpSpeed, sizeof(float), 1, _File);
    fread(&m_fLessRatio, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}