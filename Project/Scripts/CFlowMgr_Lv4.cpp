#include "pch.h"
#include "CFlowMgr_Lv4.h"

CFlowMgr_Lv4::CFlowMgr_Lv4()
    : CLevelFlowMgr(FLOWMGR_LV4)
    , m_pLight{}
    , m_eState(Lv4State::Idle)
    , m_fLightOutTime(14.f)
    , m_fAccTime(0.f)
    , m_fRatio(1.f)
{
}

CFlowMgr_Lv4::CFlowMgr_Lv4(const CFlowMgr_Lv4& _Origin)
    : CLevelFlowMgr(_Origin)
    , m_pLight{}
    , m_eState(Lv4State::Idle)
    , m_fLightOutTime(14.f)
    , m_fAccTime(0.f)
    , m_fRatio(1.f)
{
}

CFlowMgr_Lv4::~CFlowMgr_Lv4()
{
}

void CFlowMgr_Lv4::begin()
{
    CLevelFlowMgr::begin();

    SetStartLevel(false);

    LevelStart();
    SetToneMappingParam(true, false, 0.1f, 0.f);
    for (int i = 0; i < 2; i++)
    {
        CGameObject* pObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Trigger Light" + std::to_wstring(i + 1));
        if (nullptr != pObj)
        {
            m_pLight[i] = pObj->Light();
            if (m_pLight[i])
                m_pLight[i]->SetFallOffEnd(4000.f);
        }
    }

    SetFadeEffectColor(Vec3(164.f, 44.f, 174.f));
}

void CFlowMgr_Lv4::tick()
{
    CLevelFlowMgr::tick();

    switch (m_eState)
    {
    case Lv4State::Idle: {
        Idle();
    }
    break;
    case Lv4State::LightOut: {
        LightOut();
    }
    break;
    case Lv4State::End:
        break;
    default:
        break;
    }
}

void CFlowMgr_Lv4::TriggerEvent(UINT _Idx)
{
    Lv4State eEvent = Lv4State(_Idx);

    switch (eEvent)
    {
    case Lv4State::Idle: {
        m_eState = Lv4State::Idle;
    }
    break;
    case Lv4State::LightOut: {
        m_eState = Lv4State::LightOut;
    }
    break;
    default:
        break;
    }
}

void CFlowMgr_Lv4::LevelEnd()
{
    CLevelFlowMgr::LevelEnd();
    SetFadeEffectColor(Vec3(180.f, 140.f, 200.f));
}

void CFlowMgr_Lv4::LevelRestart()
{
    CLevelFlowMgr::LevelRestart();
    SetFadeEffectColor(Vec3(164.f, 44.f, 174.f));
}

void CFlowMgr_Lv4::Idle()
{
}

void CFlowMgr_Lv4::LightOut()
{
    if (m_fRatio <= 0.f)
    {
        m_eState = Lv4State::Idle;
    }

    m_fAccTime += DT;
    for (int i = 0; i < 2; i++)
    {
        if (m_pLight[i])
        {
            m_fRatio -= m_fAccTime / m_fLightOutTime;

            m_pLight[i]->SetFallOffEnd(m_pLight[i]->GetFallOffEnd() * m_fRatio);
        }
    }
}

UINT CFlowMgr_Lv4::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CFlowMgr_Lv4::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}
