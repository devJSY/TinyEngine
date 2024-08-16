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
    m_StageName[0] = "Tutorial Level";
    m_StageName[1] = "Level1-1-1";
    m_StageName[2] = "Pk3-3-1";
    m_StageName[3] = "Morpho";
    m_StageName[4] = "Elfilis";

    m_StageIconName[0] = L"fbx\\UI\\Images\\Resident\\WipeBg\\IconNormalLv1\\WipeBgIconLv1^u.png";
    m_StageIconName[1] = L"fbx\\UI\\Images\\Resident\\WipeBg\\IconNormalLv1\\WipeBgIconLv1^u.png";
    m_StageIconName[2] = L"fbx\\UI\\Images\\Resident\\WipeBg\\IconBossLv3\\WipeBgIconLv3^u.png";
    m_StageIconName[3] = L"fbx\\UI\\Images\\Wmap\\StageStartBoss\\BossWorldIconLevel7^u.png";
    m_StageIconName[4] = L"fbx\\UI\\Images\\Wmap\\StageStartBoss\\BossWorldIconLevel7^u.png";

    m_StageNameBaseName[0] = L"fbx\\UI\\Images\\Wmap\\StageStart\\StageNameBaseLevel1^u.png";
    m_StageNameBaseName[1] = L"fbx\\UI\\Images\\Wmap\\StageStart\\StageNameBaseLevel1^u.png";
    m_StageNameBaseName[2] = L"fbx\\UI\\Images\\Wmap\\StageStart\\StageNameBaseLevel3^u.png";
    m_StageNameBaseName[3] = L"fbx\\UI\\Images\\Wmap\\StageStart\\StageNameBaseLevel7^u.png";
    m_StageNameBaseName[4] = L"fbx\\UI\\Images\\Wmap\\StageStart\\StageNameBaseLevel7^u.png";

    m_pUICam = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI Camera");

    m_pLevelTex = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_RobbyLevelTex");

    m_pLevelIcon = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_RobbyLevelICon");

    m_pLevelNameBase = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_RobbyLevelNameBase");

    m_pStartBtn = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_StartBtn");

    m_pLevelIcon->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(m_StageIconName[m_iCurStage]));
    m_pLevelNameBase->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(m_StageNameBaseName[m_iCurStage]));
    SetNextLevel(m_StageName[m_iCurStage]);
}

void CFlowMgr_LvRobby::tick()
{
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
    GetFlowScript()->ChangeState(FlowState::End);
}

void CFlowMgr_LvRobby::LevelExit()
{
    CLevelFlowMgr::LevelEnd();
}

void CFlowMgr_LvRobby::EnterZoomState()
{
    switch (m_eZoomState)
    {
    case ZoomState::ZoomIn: {
        /// TODO : �ؽ��� �ٲٱ�
        m_pLevelIcon->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(m_StageIconName[m_iCurStage]));
        m_pLevelNameBase->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0,
                                                                    CAssetMgr::GetInst()->FindAsset<CTexture>(m_StageNameBaseName[m_iCurStage]));

        // TODO : SetActive(true)
        m_pLevelIcon->SetActive(true);
        m_pLevelNameBase->SetActive(true);
        m_pStartBtn->SetActive(true);
    }
    break;
    case ZoomState::ZoomOut: {
        // TODO : SetActive(false)
        m_pLevelIcon->SetActive(false);
        m_pLevelNameBase->SetActive(false);
        m_pStartBtn->SetActive(false);
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
            vWolrdPos.x -= 3000.f * DT;
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
            vWolrdPos.x += 3000.f * DT;
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
        ChangeZoomState(ZoomState::Stop);
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