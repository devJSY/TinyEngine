#include "pch.h"
#include "CFlowMgr_LvRobby.h"

#include "CUIFlowScript.h"

#include "CButtonScript.h"
#include <Engine\CKeyMgr.h>

CFlowMgr_LvRobby::CFlowMgr_LvRobby()
    : CLevelFlowMgr(FLOWMGR_LVROBBY)
    , m_eZoomState(ZoomState::Stop)
    , m_eDragState(DragState::Stop)
    , m_pLevelIcon(nullptr)
    , m_pLevelNameBase(nullptr)
    , m_pStartBtn(nullptr)
    , m_pUICam(nullptr)
    , m_pLevelTex(nullptr)
    , m_pLevelFont(nullptr)
    , m_pLevelLine(nullptr)
    , m_vPrevMousePos{}
    , m_vCurMousePos{}
    , m_StageName{}
    , m_iCurStage(0)
{
}

CFlowMgr_LvRobby::CFlowMgr_LvRobby(const CFlowMgr_LvRobby& _Origin)
    : CLevelFlowMgr(_Origin)
    , m_eZoomState(ZoomState::Stop)
    , m_eDragState(DragState::Stop)
    , m_pLevelIcon(nullptr)
    , m_pLevelNameBase(nullptr)
    , m_pStartBtn(nullptr)
    , m_pUICam(nullptr)
    , m_pLevelTex(nullptr)
    , m_pLevelFont(nullptr)
    , m_pLevelLine(nullptr)
    , m_vPrevMousePos{}
    , m_vCurMousePos{}
    , m_StageName{}
    , m_iCurStage(0)
{
}

CFlowMgr_LvRobby::~CFlowMgr_LvRobby()
{
}

void CFlowMgr_LvRobby::begin()
{
    CLevelFlowMgr::begin();

    SetUILevel(true);

    LevelStart();

    SetStartLevel(false);

    OffDimensionFade();
    SetToneMappingParam(false);
    SetUIDOFEffect();

    m_pUICam = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Camera");

    m_pLevelTex = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_RobbyLevelTex");

    m_pLevelIcon = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_RobbyLevelIcon");

    m_pLevelNameBase = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_RobbyLevelNameBase");

    m_pStartBtn = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_StartBtn");

    m_pLevelLine = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_RobbyLevelLine");

    m_pLevelFont = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_RobbyLevelNameFont");

    // Stage √ ±‚»≠
    m_StageName[0] = "Tutorial Level";
    m_StageName[1] = "Level1-1-1";
    m_StageName[2] = "Pk3-3-1";
    m_StageName[3] = "Morpho";
    m_StageName[4] = "Elfilis";

    SetNextLevel(m_StageName[m_iCurStage]);

    if (!m_pLevelIcon || !m_pStartBtn || !m_pLevelNameBase || !m_pLevelFont || !m_pLevelLine)
        return;

    // Icon
    vector<CGameObject*> pIconObjs = m_pLevelIcon->GetChildObject();
    for (size_t i = 0; i < pIconObjs.size(); i++)
    {
        if (nullptr != pIconObjs[m_iCurStage])
        {
            if (m_iCurStage == i)
                pIconObjs[i]->SetActive(true);
            else
                pIconObjs[i]->SetActive(false);
        }
    }

    // NameBase
    vector<CGameObject*> pNameBaseObjs = m_pLevelNameBase->GetChildObject();
    for (size_t i = 0; i < pNameBaseObjs.size(); i++)
    {
        if (nullptr != pNameBaseObjs[m_iCurStage])
        {
            if (m_iCurStage == i)
                pNameBaseObjs[i]->SetActive(true);
            else
                pNameBaseObjs[i]->SetActive(false);
        }
    }

    // Font
    vector<CGameObject*> pFontObjs = m_pLevelFont->GetChildObject();
    for (size_t i = 0; i < pFontObjs.size(); i++)
    {
        if (nullptr != pFontObjs[m_iCurStage])
        {
            if (m_iCurStage == i)
                pFontObjs[i]->SetActive(true);
            else
                pFontObjs[i]->SetActive(false);
        }
    }

    m_pStartBtn->SetActive(true);
    m_pLevelLine->SetActive(true);

    m_pStartBtn->GetScript<CButtonScript>()->ChangeState(ButtonState::NORMAL);
}

void CFlowMgr_LvRobby::LevelStart()
{
    ResetFadeEffectTimer();

    SetFadeEffect(Vec3(252.f, 75.f, 129.f), true, 1.f, 1.25f, true);
}

void CFlowMgr_LvRobby::tick()
{
    CLevelFlowMgr::tick();

    if (m_pStartBtn->GetScript<CButtonScript>()->IsHovered() && m_eZoomState == ZoomState::Stop && m_eDragState == DragState::Stop)
    {
        return;
    }

    int iDragValue = Drag();
    if (0 != iDragValue)
    {
        if (iDragValue == 1)
        {
            ChangeDragState(DragState::MoveMinus);
        }
        else
        {
            ChangeDragState(DragState::MovePlus);
        }

        m_iCurStage += iDragValue;

        SetNextLevel(m_StageName[m_iCurStage]);
    }
    else
    {
        if (m_eDragState == DragState::Stop && m_pUICam->Transform()->GetWorldPos().z <= -199.f && KEY_RELEASED(LBTN))
        {
            ChangeZoomState(ZoomState::ZoomIn);
        }
    }

    DragTick();
    ZoomTick();
}

void CFlowMgr_LvRobby::LevelEnd()
{
    CLevelFlowMgr::LevelEnd();

    if (0 == m_iCurStage)
    {
        // SetLoadingUIColor(Vec3(164.f, 222.f, 40.f));
        SetFadeEffect(Vec3(115.f, 156.f, 28.f), false, 1.f, 1.25f, true);
    }
    else if (1 == m_iCurStage)
    {
        SetFadeEffect(Vec3(115.f, 156.f, 28.f), false, 1.f, 1.25f, true);
    }
    else if (2 == m_iCurStage)
    {
        SetFadeEffect(Vec3(164.f, 44.f, 174.f), false, 1.f, 1.25f, true);
    }
    else if (3 == m_iCurStage)
    {
        SetFadeEffect(Vec3(255.f, 150.f, 100.f), false, 1.f, 1.25f, true);
    }
    else
    {
        SetFadeEffect(Vec3(87.f, 0.f, 150.f), false, 1.f, 1.25f, true);
    }
}

void CFlowMgr_LvRobby::LevelRestart()
{
    CLevelFlowMgr::LevelRestart();
    SetFadeEffect(Vec3(252.f, 75.f, 129.f), false, 1.f, 1.25f, true);
}

void CFlowMgr_LvRobby::RobbyLevel()
{
    return;
}

void CFlowMgr_LvRobby::EnterZoomState()
{
    if (!m_pLevelIcon || !m_pStartBtn || !m_pLevelNameBase || !m_pLevelFont || !m_pLevelLine)
        return;

    switch (m_eZoomState)
    {
    case ZoomState::ZoomIn: {
        // Icon
        vector<CGameObject*> pIconObjs = m_pLevelIcon->GetChildObject();
        for (size_t i = 0; i < pIconObjs.size(); i++)
        {
            if (nullptr != pIconObjs[m_iCurStage])
            {
                if (m_iCurStage == i)
                    pIconObjs[i]->SetActive(true);
                else
                    pIconObjs[i]->SetActive(false);
            }
        }

        // NameBase
        vector<CGameObject*> pNameBaseObjs = m_pLevelNameBase->GetChildObject();
        for (size_t i = 0; i < pNameBaseObjs.size(); i++)
        {
            if (nullptr != pNameBaseObjs[m_iCurStage])
            {
                if (m_iCurStage == i)
                    pNameBaseObjs[i]->SetActive(true);
                else
                    pNameBaseObjs[i]->SetActive(false);
            }
        }

        // Font
        vector<CGameObject*> pFontObjs = m_pLevelFont->GetChildObject();
        for (size_t i = 0; i < pFontObjs.size(); i++)
        {
            if (nullptr != pFontObjs[m_iCurStage])
            {
                if (m_iCurStage == i)
                    pFontObjs[i]->SetActive(true);
                else
                    pFontObjs[i]->SetActive(false);
            }
        }

        m_pStartBtn->SetActive(true);
        m_pLevelLine->SetActive(true);
        m_pStartBtn->GetScript<CButtonScript>()->ChangeState(ButtonState::DISABLED);
    }
    break;
    case ZoomState::ZoomOut: {
        // Icon
        vector<CGameObject*> pIconObjs = m_pLevelIcon->GetChildObject();
        for (size_t i = 0; i < pIconObjs.size(); i++)
        {
            if (nullptr != pIconObjs[m_iCurStage])
            {
                pIconObjs[i]->SetActive(false);
            }
        }

        // NameBase
        vector<CGameObject*> pNameBaseObjs = m_pLevelNameBase->GetChildObject();
        for (size_t i = 0; i < pNameBaseObjs.size(); i++)
        {
            if (nullptr != pNameBaseObjs[m_iCurStage])
            {
                pNameBaseObjs[i]->SetActive(false);
            }
        }

        // Font
        vector<CGameObject*> pFontObjs = m_pLevelFont->GetChildObject();
        for (size_t i = 0; i < pFontObjs.size(); i++)
        {
            if (nullptr != pFontObjs[m_iCurStage])
            {
                pFontObjs[i]->SetActive(false);
            }
        }

        m_pStartBtn->SetActive(false);
        m_pLevelLine->SetActive(false);
        m_pStartBtn->GetScript<CButtonScript>()->ChangeState(ButtonState::DISABLED);
    }
    break;
    case ZoomState::Stop: {
        m_pStartBtn->GetScript<CButtonScript>()->ChangeState(ButtonState::NORMAL);
    }
    break;
    case ZoomState::End:
        break;
    default:
        break;
    }
}

void CFlowMgr_LvRobby::ChangeZoomState(ZoomState _eState)
{
    ExitZoomState();
    m_eZoomState = _eState;
    EnterZoomState();
}

void CFlowMgr_LvRobby::ExitZoomState()
{
}

void CFlowMgr_LvRobby::EnterDragState()
{
}

void CFlowMgr_LvRobby::ChangeDragState(DragState _eState)
{
    ExitDragState();
    m_eDragState = _eState;
    EnterDragState();
}

void CFlowMgr_LvRobby::ExitDragState()
{
}

int CFlowMgr_LvRobby::Drag()
{
    int _iIdx = 0;

    if (KEY_TAP(LBTN))
    {
        m_vPrevMousePos = CKeyMgr::GetInst()->GetMousePos();
        ChangeZoomState(ZoomState::ZoomOut);
    }

    if (KEY_RELEASED(LBTN))
    {
        m_vCurMousePos = CKeyMgr::GetInst()->GetMousePos();

        Vec2 _vDrag = m_vCurMousePos - m_vPrevMousePos;

        if (_vDrag.x <= -1.f * 100.f)
        {
            if (m_iCurStage != 4)
            {
                _iIdx = 1;
            }
            else
            {
                ChangeZoomState(ZoomState::ZoomIn);
            }
        }

        if (_vDrag.x >= 100.f)
        {
            if (m_iCurStage != 0)
            {
                _iIdx = -1;
            }
            else
            {
                ChangeZoomState(ZoomState::ZoomIn);
            }
        }
    }

    return _iIdx;
}

void CFlowMgr_LvRobby::DragMinusMove()
{
    if (nullptr != m_pLevelTex)
    {
        CTransform* pTr = m_pLevelTex->Transform();
        Vec3 vWolrdPos = pTr->GetWorldPos();
        float vTargetPos = m_iCurStage * -4000.f;

        if (vWolrdPos.x <= vTargetPos)
        {
            vWolrdPos.x = vTargetPos;
            m_pLevelTex->Transform()->SetWorldPos(vWolrdPos);
            ChangeZoomState(ZoomState::ZoomIn);
            ChangeDragState(DragState::Stop);
        }
        else
        {
            vWolrdPos.x -= 8000.f * DT;
            m_pLevelTex->Transform()->SetWorldPos(vWolrdPos);
        }
    }
}

void CFlowMgr_LvRobby::DragPlusMove()
{
    if (nullptr != m_pLevelTex)
    {
        CTransform* pTr = m_pLevelTex->Transform();
        Vec3 vWolrdPos = pTr->GetWorldPos();
        float vTargetPos = m_iCurStage * -4000.f;

        if (vWolrdPos.x >= vTargetPos)
        {
            vWolrdPos.x = vTargetPos;
            m_pLevelTex->Transform()->SetWorldPos(vWolrdPos);
            ChangeZoomState(ZoomState::ZoomIn);
            ChangeDragState(DragState::Stop);
        }
        else
        {
            vWolrdPos.x += 8000.f * DT;
            m_pLevelTex->Transform()->SetWorldPos(vWolrdPos);
        }
    }
}

void CFlowMgr_LvRobby::ZoomOut()
{
    CTransform* pTr = m_pUICam->Transform();
    Vec3 vWolrdPos = pTr->GetWorldPos();

    if (vWolrdPos.z <= -200.f)
    {
        vWolrdPos.z = -200.f;
        m_pUICam->Transform()->SetWorldPos(vWolrdPos);
        if (!KEY_PRESSED(LBTN))
        {
            ChangeZoomState(ZoomState::ZoomIn);
        }
        else
        {
            ChangeZoomState(ZoomState::Stop);
        }
    }
    else
    {
        vWolrdPos.z -= DT * 250.f;
        m_pUICam->Transform()->SetWorldPos(vWolrdPos);
    }
}

void CFlowMgr_LvRobby::ZoomIn()
{
    CTransform* pTr = m_pUICam->Transform();
    Vec3 vWolrdPos = pTr->GetWorldPos();

    if (vWolrdPos.z >= 0.f)
    {
        vWolrdPos.z = 0.f;
        m_pUICam->Transform()->SetWorldPos(vWolrdPos);
        ChangeZoomState(ZoomState::Stop);
    }
    else
    {
        vWolrdPos.z += DT * 250.f;
        m_pUICam->Transform()->SetWorldPos(vWolrdPos);
    }
}

void CFlowMgr_LvRobby::DragTick()
{
    switch (m_eDragState)
    {
    case DragState::MoveMinus:
        DragMinusMove();
        break;
    case DragState::MovePlus:
        DragPlusMove();
        break;
    case DragState::Stop:
        break;
    case DragState::End:
        break;
    default:
        break;
    }
}

void CFlowMgr_LvRobby::ZoomTick()
{
    switch (m_eZoomState)
    {
    case ZoomState::ZoomIn: {
        ZoomIn();
    }
    break;
    case ZoomState::ZoomOut: {
        ZoomOut();
    }
    break;
    case ZoomState::Stop:
        break;
    case ZoomState::End:
        break;
    default:
        break;
    }
}

UINT CFlowMgr_LvRobby::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CFlowMgr_LvRobby::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}