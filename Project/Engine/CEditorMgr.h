#pragma once
#include "CLevelEditor.h"
#include "CMaterialEditor.h"

class CEditor;

class CEditorMgr : public CSingleton<CEditorMgr>
{
    SINGLE(CEditorMgr);

private:
    vector<CEditor*> m_vecEditor;
    Vec2 m_ViewportSize;
    Vec2 m_ViewportMousePos;
    bool m_bShowMtrlEditor;

public:
    CLevelEditor* GetLevelEditor() const { return static_cast<CLevelEditor*>(m_vecEditor[(UINT)EDITOR_TYPE::LEVEL]); }
    CMaterialEditor* GetMaterialEditor() const
    {
        return static_cast<CMaterialEditor*>(m_vecEditor[(UINT)EDITOR_TYPE::MATERIAL]);
    }

    void SetViewportSize(Vec2 size) { m_ViewportSize = size; }
    Vec2 GetViewportSize() const { return m_ViewportSize; }

    void SetViewportMousePos(Vec2 pos) { m_ViewportMousePos = pos; }
    Vec2 GetViewportMousePos() const { return m_ViewportMousePos; }

public:
    void SetShowMtrlEditor(bool _bShow) { m_bShowMtrlEditor = _bShow; }
    bool IsShowMtrlEditor() const { return m_bShowMtrlEditor; }

private:
    void SetDarkThemeColors();

public:
    void init();
    void tick();
    void render();
};
