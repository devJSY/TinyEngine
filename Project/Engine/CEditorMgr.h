#pragma once

class CEditor;

class CEditorMgr : public CSingleton<CEditorMgr>
{
    SINGLE(CEditorMgr);

private:
    vector<CEditor*> m_vecEditor;
    CEditor* m_CurEditor;
    Vec2 m_ViewportSize;

public:
    CEditor* GetCurEditor() const { return m_CurEditor; }

    void SetViewportSize(Vec2 size) { m_ViewportSize = size; }
    Vec2 GetViewportSize() const { return m_ViewportSize; }

public:
    void init();
    void tick();
    void render();
    void Resize();
};
