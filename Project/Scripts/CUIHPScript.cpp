#include "pch.h"
#include "CUIHPscript.h"

#include <Engine/CTimeMgr.h>
#include "CUnitScript.h"
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
{
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
    {
        m_pUnitScript = _pTargetObj->GetScript<CUnitScript>();
        m_pFSMScript = _pTargetObj->GetScript<CKirbyFSM>();
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

    SwitchKirbyName();
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
    AbilityCopyType eAbility = m_pFSMScript->GetCurAbilityIdx();
    ObjectCopyType eObject = m_pFSMScript->GetCurObjectIdx();

    wstring AbilityTextureName = L"";
    wstring ObjectTextureName = L"";

    switch (eAbility)
    {
    case AbilityCopyType::NORMAL: {
        AbilityTextureName = L"NormalKirby.png";
    }
    break;
    case AbilityCopyType::FIRE: {
        AbilityTextureName = L"DragonicFireKirby.png";
    }
    break;
    case AbilityCopyType::CUTTER: {
        AbilityTextureName = L"FullMetalKirby.png";
    }
    break;
    case AbilityCopyType::SWORD: {
        AbilityTextureName = L"MorphoKirby.png";
    }
    break;
    case AbilityCopyType::SLEEP: {
        AbilityTextureName = L"SleepKirby.png";
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
        ObjectTextureName = L"TriangleKirby.png";
    }
    break;
    case ObjectCopyType::VENDING_MACHINE: {
        ObjectTextureName = L"VendingMachineKirby.png";
    }
    break;
    case ObjectCopyType::LIGHT: {
        ObjectTextureName = L"BulbKirby.png";
    }
    break;
    case ObjectCopyType::END:
        break;
    default:
        break;
    }

    if (eObject == ObjectCopyType::NONE)
    {
        AbilityTextureName = L"fbx\\UI\\Images\\FontTexture\\" + AbilityTextureName;
        m_pNameObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(AbilityTextureName));
    }
    else
    {
        ObjectTextureName = L"fbx\\UI\\Images\\FontTexture\\" + ObjectTextureName;
        m_pNameObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(ObjectTextureName));
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