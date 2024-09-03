#include "pch.h"
#include "CUIHPscript.h"

#include <Engine/CTimeMgr.h>
#include "CUnitScript.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CUIHPScript::CUIHPScript()
    : CScript(UIHPSCRIPT)
    , m_vDamageTask{}
    , m_vHealTask{}
    , m_TargetName{}
    , m_pRenderer(nullptr)
    , m_pUnitScript(nullptr)
    , m_pNameObj(nullptr)
    , m_fAccTime(0.f)
    , m_fComboTime(2.f)
    , m_fHealingTime(2.f)
    , m_bIsScaling(false)
    , m_fDescSpeed(10.f)
    , m_vDecreaseColor{}
    , m_vBasicColor{}
    , m_fMaxHP(0.f)
    , m_fCurHP(0.f)
    , m_fPrevHP(0.f)
    , m_fCurPrevHP(0.f)
    , m_fSparkleAccTime(0.f)
    , m_pFSMScript(nullptr)
    , m_iEnterTickCount(0)
    , m_bDamaged(false)
    , m_bHpHealed(false)
    , m_bIsHealedScaling(false)
    , m_bSparkle(false)
    , m_bEmissionFlag(false)
    , m_bHPCurrentSparkleFlag(false)
    , m_bSoundFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_TargetName, "TargetName");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vBasicColor, "Basic Color");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_vDecreaseColor, "Decrease Color");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fComboTime, "ComboTime");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDescSpeed, "DecreaseSpeed");
}

CUIHPScript::CUIHPScript(const CUIHPScript& Origin)
    : CScript(Origin)
    , m_vDamageTask{}
    , m_vHealTask{}
    , m_TargetName{}
    , m_pRenderer(nullptr)
    , m_pUnitScript(nullptr)
    , m_pNameObj(nullptr)
    , m_fAccTime(0.f)
    , m_fHealingTime(Origin.m_fHealingTime)
    , m_fComboTime(Origin.m_fComboTime)
    , m_bIsScaling(false)
    , m_fDescSpeed(10.f)
    , m_vDecreaseColor(Origin.m_vDecreaseColor)
    , m_vBasicColor(Origin.m_vBasicColor)
    , m_fMaxHP(0.f)
    , m_fCurHP(0.f)
    , m_fPrevHP(0.f)
    , m_fCurPrevHP(0.f)
    , m_fSparkleAccTime(0.f)
    , m_pFSMScript(nullptr)
    , m_iEnterTickCount(0)
    , m_bDamaged(false)
    , m_bHpHealed(false)
    , m_bIsHealedScaling(false)
    , m_bSparkle(false)
    , m_bEmissionFlag(false)
    , m_bHPCurrentSparkleFlag(false)
    , m_bSoundFlag(false)
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

void CUIHPScript::SetPlayer()
{
    if (PLAYER)
    {
        m_pUnitScript = PLAYER->GetScript<CUnitScript>();
        m_pFSMScript = PLAYER->GetScript<CKirbyFSM>();
    }
}

void CUIHPScript::tick()
{
    if (!m_pUnitScript)
        return;

    SetInitInfo();

    if (m_iEnterTickCount <= 1)
    {
        m_iEnterTickCount++;
        return;
    }

    // 데미지가 달면 현재 체력과 달라짐
    float fCheckHP = m_fCurHP;
    m_fCurHP = m_pUnitScript->GetCurInfo().HP;

    // 현재 체력 상태에 따른 Flag 점화
    if (m_fCurHP / m_fMaxHP <= 0.25f)
    {
        m_bHPCurrentSparkleFlag = true;
        if (!m_bSoundFlag)
        {
            m_bSoundFlag = true;
            GamePlayStatic::Play2DSound(L"sound\\wav\\UiBasic\\0001.wav", 1, 0.3f);
        }
    }
    else
    {
        m_bSoundFlag = false;
        m_bHPCurrentSparkleFlag = false;
    }

    // 뭐든 현재 체력의 변화가 생김
    if (abs(fCheckHP - m_fCurHP) >= 0.1f)
    {
        // 체력의 변화가 생겼다는 것을 감지
        // 데미지를 받음
        if (fCheckHP - m_fCurHP > 0.f)
        {
            m_vDamageTask.push_back({m_fCurHP, fCheckHP});
            m_bDamaged = true;
            m_bSparkle = true;

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
                m_bSparkle = true;
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

    SwitchKirbyName();
    Sparkle();
}

void CUIHPScript::SetInitInfo()
{
    if (1 != m_iEnterTickCount)
        return;

    m_pNameObj = GetOwner()->GetChildObject(L"UI_PlayerName");

    if (!m_pUnitScript)
        return;

    m_fMaxHP = PLAYER->GetScript<CUnitScript>()->GetCurInfo().MAXHP;
    m_fCurPrevHP = m_fPrevHP = m_fCurHP = PLAYER->GetScript<CUnitScript>()->GetCurInfo().HP;

    m_pRenderer = GetOwner()->GetComponent<CMeshRender>();
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_0, m_fCurHP / m_fMaxHP);
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_1, m_fPrevHP / m_fMaxHP);
    m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_0, m_vBasicColor);
    m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_1, m_vDecreaseColor);
    m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_3, Vec4(1.f, 1.f, 1.f, 1.f));

    m_bSparkle = false;
    m_bHPCurrentSparkleFlag = false;
    m_bDamaged = false;
    m_bHpHealed = false;
}

void CUIHPScript::CaculateShading()
{
    float _fShadingRatio = m_fCurHP / m_fMaxHP;
    m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_1, Vec4(255.f, 255.f, 0.f, 255.f) / 255.f);
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_0, _fShadingRatio);
}

void CUIHPScript::CaculateHealShading()
{
    float _fShadingRatio = m_fPrevHP / m_fMaxHP;
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_0, _fShadingRatio);
    m_bIsHealedScaling = false;
}

void CUIHPScript::HealScaling()
{
    m_fPrevHP += CTimeMgr::GetInst()->GetDeltaTime() * m_fDescSpeed;

    if (m_fCurHP <= m_fPrevHP)
    {
        m_fPrevHP = m_fCurHP;
        m_bIsHealedScaling = false;
    }
    m_pRenderer->GetMaterial(0)->SetScalarParam(VEC4_1, Vec4(255.f, 182.f, 206.f, 255.f) / 255.f);
    float _fScalingRatio = m_fPrevHP / m_fMaxHP;
    m_pRenderer->GetMaterial(0)->SetScalarParam(FLOAT_1, _fScalingRatio);
}

void CUIHPScript::Scaling()
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
        for (int i = 0; i < 3; i++)
        {
            if (GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i)))
                GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i))->SetActive(false);
        }

        for (int i = 3; i < 8; i++)
        {
            if (i == AbilityIdx)
            {
                if (GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i)))
                    GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i))->SetActive(true);
            }
            else
            {
                if (GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i)))
                    GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i))->SetActive(false);
            }
        }
    }
    else
    {
        for (int i = 3; i < 8; i++)
        {
            if (GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i)))
                GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i))->SetActive(false);
        }

        for (int i = 0; i < 3; i++)
        {
            if (i == ObjectIdx)
            {
                if (GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i)))
                    GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i))->SetActive(true);
            }
            else
            {
                if (GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i)))
                    GetOwner()->GetChildObject(L"UI_PlayerName" + std::to_wstring(i))->SetActive(false);
            }
        }
    }
}

void CUIHPScript::HPDamageTask()
{
    for (size_t i = 0; i < m_vDamageTask.size(); ++i)
    {
        // 계속해서 데미지가 들어오는지 확인
        // 둘의 차이가 있다는 것은 곧 데미지가 들어왔다는 의미
        // Scaling() 시간 초기화
        if (m_fCurPrevHP > m_vDamageTask[i].fCurHP)
        {
            m_fAccTime = 0.f;
            m_fCurPrevHP = m_vDamageTask[i].fCurHP;
        }

        // 가장 큰 전 HP 구하기
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

void CUIHPScript::HPHealTask()
{
    for (size_t i = 0; i < m_vHealTask.size(); ++i)
    {
        // 계속해서 흡혈이 들어오는지 확인
        // 둘의 차이가 있다는 것은 곧 흡혈 했다는 의미
        // Scaling() 시간 초기화
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

void CUIHPScript::Sparkle()
{
    Vec4 vEmissionColor = {};
    if (m_bSparkle || m_bHPCurrentSparkleFlag)
    {
        m_fSparkleAccTime += DT;
        m_fSparkleTermTime += DT;

        if (0.08f <= m_fSparkleTermTime)
        {
            if (m_bEmissionFlag)
            {
                vEmissionColor = Vec4(1.4f, 1.4f, 1.4f, 1.f);
            }
            else
            {
                vEmissionColor = Vec4(1.f, 1.f, 1.f, 1.f);
            }
            m_fSparkleTermTime = 0.f;
            m_bEmissionFlag = !m_bEmissionFlag;

            GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_3, vEmissionColor);
        }

        if (2.5f <= m_fSparkleAccTime)
        {
            SparKleReset();
        }
    }
}

void CUIHPScript::SparKleReset()
{
    m_fSparkleAccTime = 0.f;
    m_bSparkle = false;
    m_bEmissionFlag = true;
    m_bIsAlreadyFlag = false;
    GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_3, Vec4(1.f, 1.f, 1.f, 1.f));
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