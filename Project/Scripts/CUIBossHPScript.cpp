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

void CUIBossHPScript::begin()
{
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
        HPTick();
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
    m_eState = _state;
    EnterState();
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
        m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_3, Vec4(1.f, 1.f, 1.f, 1.f));

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
    m_fEnterHP += DT * 700.f;

    float _fScalingRatio = m_fEnterHP / m_fMaxHP;

    if (m_fEnterHP >= m_fMaxHP)
    {
        m_fCurHP = m_fPrevHP = m_fMaxHP;
        ChangeState(HPState::Tick);
    }

    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_1, _fScalingRatio);
}

void CUIBossHPScript::HPTick()
{
    if (!m_pUnitScript)
        return;

    // 데미지가 달면 현재 체력과 달라짐
    float fCheckHP = m_fCurHP;
    m_fCurHP = m_pUnitScript->GetCurInfo().HP;

    // 뭐든 현재 체력의 변화가 생김
    if (abs(fCheckHP - m_fCurHP) >= 0.1f)
    {
        // 체력의 변화가 생겼다는 것을 감지
        // 데미지를 받음
        if (fCheckHP - m_fCurHP > 0.f)
        {
            m_vDamageTask.push_back({m_fCurHP, fCheckHP});
            m_bDamaged = true;

            if (m_bHpHealed)
            {
                m_bHpHealed = false;
                m_fCurPrevHP = m_fPrevHP = m_fCurHP;
            }
        }
        // 체력을 회복함
        else if (fCheckHP - m_fCurHP < 0.f)
        {
            m_vHealTask.push_back({m_fCurHP, fCheckHP});
            m_bHpHealed = true;

            if (m_bDamaged)
            {
                m_bDamaged = false;
                m_fCurPrevHP = m_fPrevHP = m_fCurHP;
            }
        }
    }

    if (m_bDamaged)
    {
        HPDamageTask();
    }

    if (m_bHpHealed)
    {
        HealScaling();
        HPHealTask();
    }

    if (m_bIsScaling)
    {
        Scaling();
    }

    if (m_bIsHealedScaling)
    {
        CaculateHealShading();
    }
}

void CUIBossHPScript::End()
{
    GetOwner()->SetActive(false);
}

void CUIBossHPScript::CaculateShading()
{
    float _fShadingRatio = m_fCurHP / m_fMaxHP;
    m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_1, Vec4(185.f, 107.f, 15.f, 255.f) / 255.f);
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_0, _fShadingRatio);
}

void CUIBossHPScript::CaculateHealShading()
{
    float _fShadingRatio = m_fPrevHP / m_fMaxHP;
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_0, _fShadingRatio);
    m_bIsHealedScaling = false;
}

void CUIBossHPScript::HealScaling()
{
    m_fPrevHP += CTimeMgr::GetInst()->GetDeltaTime() * m_fDescSpeed;

    if (m_fCurHP <= m_fPrevHP)
    {
        m_fPrevHP = m_fCurHP;
        m_bIsHealedScaling = false;
    }
    m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_1, Vec4(0.f, 176.f, 151.f, 255.f) / 255.f);
    float _fScalingRatio = m_fPrevHP / m_fMaxHP;
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_1, _fScalingRatio);
}

void CUIBossHPScript::Scaling()
{
    m_fPrevHP -= CTimeMgr::GetInst()->GetDeltaTime() * m_fDescSpeed;

    if (m_fCurHP >= m_fPrevHP)
    {
        m_fCurPrevHP = m_fPrevHP = m_fCurHP;
        m_bIsScaling = false;
    }

    float _fScalingRatio = m_fPrevHP / m_fMaxHP;
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_1, _fScalingRatio);
}

void CUIBossHPScript::HPDamageTask()
{
    for (size_t i = 0; i < m_vDamageTask.size(); ++i)
    {
        // m_fPrevHP는 시작 할 때 현재 HP를 복사한다.
        if (m_fCurPrevHP > m_vDamageTask[i].fCurHP)
        {
            m_fAccTime = 0.f;
            m_fCurPrevHP = m_vDamageTask[i].fCurHP;
        }

        if (m_fPrevHP < m_vDamageTask[i].fPrevHP)
        {
            m_fPrevHP = m_vDamageTask[i].fPrevHP;
        }
    }

    m_fAccTime += DT;
    if (m_fAccTime >= m_fComboTime)
    {
        m_fAccTime = 0.f;
        m_bDamaged = false;
        m_bIsScaling = true;
        m_vDamageTask.clear();
    }

    CaculateShading();
}

void CUIBossHPScript::HPHealTask()
{
    for (size_t i = 0; i < m_vHealTask.size(); ++i)
    {
        // m_fPrevHP는 시작 할 때 현재 HP를 복사한다.
        if (m_fCurPrevHP < m_vHealTask[i].fCurHP)
        {
            m_fAccTime = 0.f;
            m_fCurPrevHP = m_vHealTask[i].fCurHP;
        }
    }

    m_fAccTime += DT;
    if (m_fAccTime >= m_fComboTime)
    {
        m_fAccTime = 0.f;
        m_bHpHealed = false;
        m_bIsScaling = false;
        m_bIsHealedScaling = true;
        m_vHealTask.clear();
    }
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