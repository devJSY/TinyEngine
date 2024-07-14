#include "pch.h"
#include "CUIHPscript.h"

#include <Engine/CTimeMgr.h>
#include "CUnitScript.h"

CUIHPScript::CUIHPScript()
    : CScript(UIHPSCRIPT)
    , m_pSlicingObjects{}
    , m_TargetName{}
    , m_pTr(nullptr)
    , m_pRenderer(nullptr)
    , m_pUnitScript(nullptr)
    , m_vNormalScale{}
    , m_vNormalPos{}
    , m_fAccTime(0.f)
    , m_fComboTime(2.f)
    , m_bIsCombo(false)
    , m_bIsScaling(false)
    , m_fDescSpeed(10.f)
    , m_vDecreaseColor{}
    , m_vBasicColor{}
    , m_fMaxHP(400.f)
    , m_fCurHP(400.f)
    , m_fPrevHP(400.f)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_TargetName, "TargetName");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vBasicColor, "Basic Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vDecreaseColor, "Decrease Color");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fComboTime, "ComboTime");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDescSpeed, "DecreaseSpeed");
}

CUIHPScript::~CUIHPScript()
{
}

void CUIHPScript::begin()
{
    CGameObject* _pTargetObj = nullptr;
    if (!m_pUnitScript && m_TargetName != "")
        _pTargetObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(ToWstring(m_TargetName));
        
    if (_pTargetObj)
        m_pUnitScript = _pTargetObj->GetScript<CUnitScript>();

    if (!m_pUnitScript)
        return;

    m_fMaxHP = m_pUnitScript->GetCurInfo().MAXHP;
    m_fCurHP = m_pUnitScript->GetCurInfo().HP;
    m_fPrevHP = m_pUnitScript->GetCurInfo().HP;

    m_pTr = GetOwner()->GetComponent<CTransform>();
    m_pRenderer = GetOwner()->GetComponent<CMeshRender>();
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_0, 1.f);
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_1, 1.f);
    m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_0, m_vBasicColor);
    m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_1, m_vDecreaseColor);

    m_vNormalScale = m_pTr->GetLocalScale();
    m_vNormalPos = m_pTr->GetLocalPos();

    // m_fMaxHP = m_fCurHP = m_fPrevH = TargetObjectÀÇ MaxHP!
}

void CUIHPScript::tick()
{
    if (!m_pUnitScript)
        return;

    m_fCurHP = m_pUnitScript->GetCurInfo().HP;

    if (m_bIsCombo)
    {
        CaculateShading();
    }

    bool _IsCombo = IsCombo();

    if (!_IsCombo && m_bIsCombo)
    {
        m_bIsScaling = true;
    }

    if (m_bIsScaling)
        Scaling();
}

void CUIHPScript::CaculateShading()
{
    float _fShadingRatio = m_fCurHP / m_fMaxHP;
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_0, _fShadingRatio);
}

void CUIHPScript::Scaling()
{
    m_bIsCombo = false;

    m_fPrevHP -= CTimeMgr::GetInst()->GetDeltaTime() * m_fDescSpeed;

    if (m_fCurHP >= m_fPrevHP)
    {
        m_fPrevHP = m_fCurHP;
    }

    float _fScalingRatio = m_fPrevHP / m_fMaxHP;
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_1, _fScalingRatio);

    if (m_fCurHP >= m_fPrevHP)
    {
        m_bIsScaling = false;
    }
}

bool CUIHPScript::IsCombo()
{
    bool _IsCombo = m_pUnitScript->IsGetDamage();

    // Test¿ë
    if (_IsCombo)
    {
        if (!m_bIsCombo)
        {
            m_bIsCombo = true;
            m_bIsScaling = false;
        }
    }

    _IsCombo == true ? m_fAccTime = 0.f : m_fAccTime += CTimeMgr::GetInst()->GetDeltaTime();

    if (m_fAccTime >= m_fComboTime)
    {
        _IsCombo = false;
    }
    else
    {
        _IsCombo = true;
    }

    return _IsCombo;
}

void CUIHPScript::SaveToLevelFile(FILE* _File)
{
    SaveWStringToFile(ToWstring(m_TargetName), _File);
    fwrite(&m_vBasicColor, sizeof(Vec4), 1, _File);
    fwrite(&m_vDecreaseColor, sizeof(Vec4), 1, _File);
    fwrite(&m_fComboTime, sizeof(float), 1, _File);
    fwrite(&m_fDescSpeed, sizeof(float), 1, _File);
}

void CUIHPScript::LoadFromLevelFile(FILE* _File)
{
    wstring _TargetName = {};
    LoadWStringFromFile(_TargetName, _File);
    fread(&m_vBasicColor, sizeof(Vec4), 1, _File);
    fread(&m_vDecreaseColor, sizeof(Vec4), 1, _File);
    fread(&m_fComboTime, sizeof(float), 1, _File);
    fread(&m_fDescSpeed, sizeof(float), 1, _File);
    
    m_TargetName = ToString(_TargetName);
}