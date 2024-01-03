#include "pch.h"
#include "CEditorMgr.h"
#include "CEditor.h"
#include "CLevelEditor.h"

CEditorMgr::CEditorMgr()
    : m_vecEditor{}
    , m_CurEditor(nullptr)
    , m_ViewportSize(Vec2())
    , m_ViewportMousePos(Vec2())
{
}

CEditorMgr::~CEditorMgr()
{
    Delete_Vec(m_vecEditor);
}

void CEditorMgr::init()
{
    m_vecEditor.resize((UINT)EDITOR_TYPE::END);
    m_vecEditor[(UINT)EDITOR_TYPE::LEVEL] = new CLevelEditor;
    m_vecEditor[(UINT)EDITOR_TYPE::LEVEL]->init();

    m_CurEditor = m_vecEditor[(UINT)EDITOR_TYPE::LEVEL];
}

void CEditorMgr::tick()
{
    if (nullptr == m_CurEditor)
        return;

    m_CurEditor->tick();
    m_CurEditor->finaltick();
}

void CEditorMgr::render()
{
    if (nullptr == m_CurEditor)
        return;

    m_CurEditor->render();
}