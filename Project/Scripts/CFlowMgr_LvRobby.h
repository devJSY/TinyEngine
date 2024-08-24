#pragma once

#include "CLevelFlowMgr.h"

enum class ZoomState
{
    ZoomIn,
    ZoomOut,
    Stop,
    End,
};

enum class DragState
{
    MovePlus,
    MoveMinus,
    Stop,
    End,
};

class CFlowMgr_LvRobby : public CLevelFlowMgr
{
private:
    ZoomState m_eZoomState;
    DragState m_eDragState;

    CGameObject* m_pLevelTex;

    CGameObject* m_pLevelIcon;
    CGameObject* m_pLevelNameBase;
    CGameObject* m_pStartBtn;
    CGameObject* m_pLevelFont;
    CGameObject* m_pLevelLine;

    CGameObject* m_pUICam;

    int m_iCurStage;
    string m_StageName[5];
    wstring m_StageIconName[5];
    wstring m_StageNameBaseName[5];

    bool m_bIsMouseEvent;

    Vec2 m_vPrevMousePos;
    Vec2 m_vCurMousePos;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void LevelStart() override;
    virtual void LevelEnd() override;
    virtual void LevelRestart() override;

private:
    void EnterZoomState();
    void ChangeZoomState(ZoomState _eState);
    void ExitZoomState();

    void EnterDragState();
    void ChangeDragState(DragState _eState);
    void ExitDragState();

private:
    int Drag();
    void DragMinusMove();
    void DragPlusMove();

    void ZoomOut();
    void ZoomIn();

private:
    void DragTick();
    void ZoomTick();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CFlowMgr_LvRobby);
    CFlowMgr_LvRobby();
    CFlowMgr_LvRobby(const CFlowMgr_LvRobby& _Origin);
    virtual ~CFlowMgr_LvRobby();
};
