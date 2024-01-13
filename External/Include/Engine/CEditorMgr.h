#pragma once
#include "CLevelEditor.h"

class CEditor;

class CEditorMgr : public CSingleton<CEditorMgr>
{
    SINGLE(CEditorMgr);

private:
    vector<CEditor*> m_vecEditor;
    Vec2 m_ViewportSize;
    Vec2 m_ViewportMousePos;

public:
    CLevelEditor* GetLevelEditor() const { return static_cast<CLevelEditor*>(m_vecEditor[(UINT)EDITOR_TYPE::LEVEL]); }
    int AddEditor(CEditor* _Editor);
    int DeleteEditor(CEditor* _Editor);

    void SetViewportSize(Vec2 size) { m_ViewportSize = size; }
    Vec2 GetViewportSize() const { return m_ViewportSize; }

    void SetViewportMousePos(Vec2 pos) { m_ViewportMousePos = pos; }
    Vec2 GetViewportMousePos() const { return m_ViewportMousePos; }

private:
    void SetDarkThemeColors();

public:
    void init();
    void tick();
    void render();
};
