#include "pch.h"
#include "CUIBossHPScript.h"

#include "CUnitScript.h"

CUIBossHPScript::CUIBossHPScript()
    : CScript(UIBOSSHPSCRIPT)
    , m_pNameObj(nullptr)
    , m_pNameObj2(nullptr)
    , m_TargetName{}
    , m_pUnitScript(nullptr)
    , m_pRenderer(nullptr)
    , m_fAccTime(0.f)
    , m_fComboTime(2.f)
    , m_bIsCombo(false)
    , m_bIsScaling(false)
    , m_fDescSpeed(10.f)
    , m_vDecreaseColor{}
    , m_vBasicColor{}
    , m_fMaxHP(0.f)
    , m_fCurHP(0.f)
    , m_fPrevHP(0.f)
    , m_fEnterHP(0.f)
    , m_bMolPho(false)
    , m_bElfilis(false)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bMolPho, "Molpho");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bElfilis, "Elfilis");
    AddScriptParam(SCRIPT_PARAM::STRING, &m_TargetName, "TargetName");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vBasicColor, "Basic Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vDecreaseColor, "Decrease Color");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fComboTime, "ComboTime");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDescSpeed, "DecreaseSpeed");
}

CUIBossHPScript::CUIBossHPScript(const CUIBossHPScript& Origin)
    : CScript(Origin)
    , m_pNameObj(nullptr)
    , m_pNameObj2(nullptr)
    , m_TargetName{}
    , m_pUnitScript(nullptr)
    , m_pRenderer(nullptr)
    , m_fAccTime(0.f)
    , m_fComboTime(Origin.m_fComboTime)
    , m_bIsCombo(false)
    , m_bIsScaling(false)
    , m_fDescSpeed(Origin.m_fDescSpeed)
    , m_vDecreaseColor(Origin.m_vDecreaseColor)
    , m_vBasicColor(Origin.m_vBasicColor)
    , m_fMaxHP(0.f)
    , m_fCurHP(0.f)
    , m_fPrevHP(0.f)
    , m_fEnterHP(0.f)
    , m_bMolPho(false)
    , m_bElfilis(false)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bMolPho, "Molpho");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bElfilis, "Elfilis");
    AddScriptParam(SCRIPT_PARAM::STRING, &m_TargetName, "TargetName");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vBasicColor, "Basic Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vDecreaseColor, "Decrease Color");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fComboTime, "ComboTime");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDescSpeed, "DecreaseSpeed");
}

CUIBossHPScript::~CUIBossHPScript()
{
}

// UI_BossName1
// UI_BossName2

void CUIBossHPScript::begin()
{
    ChangeState(HPState::Enter);
}

void CUIBossHPScript::tick()
{
    switch (m_eState)
    {
    case HPState::Enter: {
        Enter();
    }
    break;
    case HPState::Tick: {
        Tick();
    }
    break;
    case HPState::End: {
        End();
    }
    break;
    default:
        break;
    }
}

void CUIBossHPScript::ChangeState(HPState _state)
{
    EnterState();
    m_eState = _state;
}

void CUIBossHPScript::EnterState()
{
    switch (m_eState)
    {
    case HPState::Enter: {
        CGameObject* _pTargetObj = nullptr;
        if (!m_pUnitScript && m_TargetName != "")
            _pTargetObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(ToWstring(m_TargetName));

        if (_pTargetObj)
        {
            m_pUnitScript = _pTargetObj->GetScript<CUnitScript>();
        }

        m_pNameObj = GetOwner()->GetChildObject(L"UI_PlayerName");

        if (!m_pUnitScript)
            return;

        m_fMaxHP = m_pUnitScript->GetCurInfo().MAXHP;
        m_fCurHP = m_pUnitScript->GetCurInfo().HP;
        m_fPrevHP = m_pUnitScript->GetCurInfo().HP;

        m_pRenderer = GetOwner()->GetComponent<CMeshRender>();
        m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_0, m_fCurHP / m_fMaxHP);
        m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_1, m_fPrevHP / m_fMaxHP);
        m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_0, m_vBasicColor);
        m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_1, m_vDecreaseColor);

        m_pNameObj = GetOwner()->GetChildObject(L"UI_BossName1");
        m_pNameObj2 = GetOwner()->GetChildObject(L"UI_BossName2");

        if (m_bMolPho)
        {
            m_pNameObj->SetActive(true);
            m_pNameObj2->SetActive(false);
        }
        else if (m_bElfilis)
        {
            m_pNameObj->SetActive(false);
            m_pNameObj2->SetActive(true);
        }
    }
    break;
    case HPState::Tick:
        break;
    case HPState::End:
        break;
    default:
        break;
    }
}

void CUIBossHPScript::Enter()
{
    m_fEnterHP += DT * 20.f;

    float _fScalingRatio = m_fEnterHP / m_fMaxHP;

    if (m_fEnterHP >= m_fMaxHP)
    {
        m_fCurHP = m_fPrevHP = m_fMaxHP;
        ChangeState(HPState::Tick);
    }

    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_1, _fScalingRatio);
}

void CUIBossHPScript::Tick()
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

void CUIBossHPScript::End()
{
    GetOwner()->SetActive(false);
}

void CUIBossHPScript::CaculateShading()
{
    float _fShadingRatio = m_fCurHP / m_fMaxHP;
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_0, _fShadingRatio);
}

void CUIBossHPScript::Scaling()
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

bool CUIBossHPScript::IsCombo()
{
    bool _IsCombo = m_pUnitScript->IsGetDamage();

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

UINT CUIBossHPScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += SaveWStringToFile(ToWstring(m_TargetName), _File);
    fwrite(&m_vBasicColor, sizeof(Vec4), 1, _File);
    fwrite(&m_vDecreaseColor, sizeof(Vec4), 1, _File);
    fwrite(&m_fComboTime, sizeof(float), 1, _File);
    fwrite(&m_fDescSpeed, sizeof(float), 1, _File);
    fwrite(&m_bMolPho, sizeof(bool), 1, _File);
    fwrite(&m_bElfilis, sizeof(bool), 1, _File);

    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CUIBossHPScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring _TargetName = {};
    MemoryByte += LoadWStringFromFile(_TargetName, _File);
    fread(&m_vBasicColor, sizeof(Vec4), 1, _File);
    fread(&m_vDecreaseColor, sizeof(Vec4), 1, _File);
    fread(&m_fComboTime, sizeof(float), 1, _File);
    fread(&m_fDescSpeed, sizeof(float), 1, _File);
    fread(&m_bMolPho, sizeof(bool), 1, _File);
    fread(&m_bElfilis, sizeof(bool), 1, _File);

    m_TargetName = ToString(_TargetName);

    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}