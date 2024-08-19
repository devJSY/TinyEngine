#include "pch.h"
#include "CUIHPscript.h"

#include <Engine/CTimeMgr.h>
#include "CUnitScript.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CUIHPScript::CUIHPScript()
    : CScript(UIHPSCRIPT)
    , m_TargetName{}
    , m_pRenderer(nullptr)
    , m_pUnitScript(nullptr)
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
    , m_pFSMScript(nullptr)
    , m_bIsEnter(true)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_TargetName, "TargetName");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vBasicColor, "Basic Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vDecreaseColor, "Decrease Color");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fComboTime, "ComboTime");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDescSpeed, "DecreaseSpeed");
}

CUIHPScript::CUIHPScript(const CUIHPScript& Origin)
    : CScript(Origin)
    , m_TargetName{}
    , m_pRenderer(nullptr)
    , m_pUnitScript(nullptr)
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
    , m_pFSMScript(nullptr)
    , m_bIsEnter(true)
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
    // m_fMaxHP = m_fCurHP = m_fPrevH = TargetObjectÀÇ MaxHP!
}

void CUIHPScript::SetPlayer()
{
    if (PLAYER)
    {
        m_pUnitScript = PLAYER->GetScript<CUnitScript>();
        m_pFSMScript = PLAYER->GetScript<CKirbyFSM>();
        m_bIsEnter = true;
    }
}

void CUIHPScript::tick()
{
    if (!m_pUnitScript)
        return;

    SetInitInfo();

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

    SwitchKirbyName();
}

void CUIHPScript::SetInitInfo()
{
    if (!m_bIsEnter)
        return;

    m_pNameObj = GetOwner()->GetChildObject(L"UI_PlayerName");

    if (!m_pUnitScript)
        return;

    m_pRenderer = GetOwner()->GetComponent<CMeshRender>();
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_0, m_fCurHP / m_fMaxHP);
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_1, m_fPrevHP / m_fMaxHP);
    m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_0, m_vBasicColor);
    m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_1, m_vDecreaseColor);

    m_fMaxHP = PLAYER->GetScript<CUnitScript>()->GetCurInfo().MAXHP;
    m_fPrevHP = m_fCurHP = PLAYER->GetScript<CUnitScript>()->GetCurInfo().HP;

    m_bIsEnter = false;
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

void CUIHPScript::SwitchKirbyName()
{
    if (nullptr == m_pFSMScript)
        return;

    AbilityCopyType eAbility = m_pFSMScript->GetCurAbilityIdx();
    ObjectCopyType eObject = m_pFSMScript->GetCurObjectIdx();

    int AbilityIdx = -1;
    int ObjectIdx = -1;

    switch (eAbility)
    {
    case AbilityCopyType::NORMAL: {
        AbilityIdx = 3;
    }
    break;
    case AbilityCopyType::FIRE: {
        AbilityIdx = 6;
    }
    break;
    case AbilityCopyType::CUTTER: {
        AbilityIdx = 5;
    }
    break;
    case AbilityCopyType::SWORD: {
        AbilityIdx = 4;
    }
    break;
    case AbilityCopyType::SLEEP: {
        AbilityIdx = 7;
    }
    break;
    case AbilityCopyType::END:
        break;
    default:
        break;
    }

    switch (eObject)
    {
    case ObjectCopyType::NONE: {
    }
    break;
    case ObjectCopyType::CONE: {
        ObjectIdx = 0;
    }
    break;
    case ObjectCopyType::VENDING_MACHINE: {
        ObjectIdx = 1;
    }
    break;
    case ObjectCopyType::LIGHT: {
        ObjectIdx = 2;
    }
    break;
    case ObjectCopyType::END:
        break;
    default:
        break;
    }

    if (-1 == ObjectIdx)
    {
        for (int i = 3; i < 8; i++)
        {
            if (i == AbilityIdx)
                GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i))->SetActive(true);
            else
                GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i))->SetActive(false);
        }

        for (int i = 0; i < 3; i++)
        {
            GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i))->SetActive(false);
        }
    }
    else
    {
        for (int i = 3; i < 8; i++)
        {
            GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i))->SetActive(false);
        }

        for (int i = 0; i < 3; i++)
        {
            if (i == ObjectIdx)
                GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i))->SetActive(true);
            else
                GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i))->SetActive(false);
        }
    }
}

bool CUIHPScript::IsCombo()
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

UINT CUIHPScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += SaveWStringToFile(ToWstring(m_TargetName), _File);
    fwrite(&m_vBasicColor, sizeof(Vec4), 1, _File);
    fwrite(&m_vDecreaseColor, sizeof(Vec4), 1, _File);
    fwrite(&m_fComboTime, sizeof(float), 1, _File);
    fwrite(&m_fDescSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CUIHPScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring _TargetName = {};
    MemoryByte += LoadWStringFromFile(_TargetName, _File);
    fread(&m_vBasicColor, sizeof(Vec4), 1, _File);
    fread(&m_vDecreaseColor, sizeof(Vec4), 1, _File);
    fread(&m_fComboTime, sizeof(float), 1, _File);
    fread(&m_fDescSpeed, sizeof(float), 1, _File);

    m_TargetName = ToString(_TargetName);

    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(Vec4);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}