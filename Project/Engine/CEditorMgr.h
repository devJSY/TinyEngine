#pragma once

class CEditor;

class CEditorMgr : public CSingleton<CEditorMgr>
{
    SINGLE(CEditorMgr);

private:
    vector<CEditor*> m_vecEditor;
    CEditor* m_CurEditor;
    Vec2 m_ViewportSize;
    Vec2 m_ViewportMousePos;

public:
    CEditor* GetCurEditor() const { return m_CurEditor; }

    void SetViewportSize(Vec2 size) { m_ViewportSize = size; }
    Vec2 GetViewportSize() const { return m_ViewportSize; }

    void SetViewportMousePos(Vec2 pos) { m_ViewportMousePos = pos; }
    Vec2 GetViewportMousePos() const { return m_ViewportMousePos; }

public:
    void init();
    void tick();
    void render();
};
