#pragma once

class CEditor;

class CEditorMgr : public CSingleton<CEditorMgr>
{
    SINGLE(CEditorMgr);

private:
    vector<CEditor*> m_vecEditor;
    CEditor* m_CurEditor;

public:
    void init();
    void tick();
    void render();
};
