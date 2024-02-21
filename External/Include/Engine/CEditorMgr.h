#pragma once
#include "CLevelEditor.h"
#include "CMaterialEditor.h"
#include "CBlueprintEditor.h"
#include "CSpriteEditor.h"
#include "CTileMapEditor.h"

class CEditor;
class CGameObject;
class CGameObjectEx;

class CEditorMgr : public CSingleton<CEditorMgr>
{
    SINGLE(CEditorMgr);

private:
    bool m_bEnable; // 에디터 활성화/비활성화
    CEditor* m_arrEditor[(UINT)EDITOR_TYPE::END];

    CGameObject* m_SelectedObj;

    Vec2 m_ViewportSize;
    Vec2 m_ViewportMousePos;

    vector<CGameObjectEx*> m_vecEditorObj;

public:
    bool IsEnable() const { return m_bEnable; }

    CLevelEditor* GetLevelEditor() const { return (CLevelEditor*)m_arrEditor[(UINT)EDITOR_TYPE::LEVEL]; }
    CMaterialEditor* GetMaterialEditor() const { return (CMaterialEditor*)m_arrEditor[(UINT)EDITOR_TYPE::MATERIAL]; }
    CBlueprintEditor* GetBlueprintEditor() const { return (CBlueprintEditor*)m_arrEditor[(UINT)EDITOR_TYPE::BLUEPRINT]; }
    CSpriteEditor* GetSpriteEditor() const { return (CSpriteEditor*)m_arrEditor[(UINT)EDITOR_TYPE::SPRITE]; }
    CTileMapEditor* GetTileMapEditor() const { return (CTileMapEditor*)m_arrEditor[(UINT)EDITOR_TYPE::TILEMAP]; }

    CGameObject* GetSelectedObject() const { return m_SelectedObj; }
    void SetSelectedObject(CGameObject* obj) { m_SelectedObj = obj; }

    void SetViewportSize(Vec2 size) { m_ViewportSize = size; }
    Vec2 GetViewportSize() const { return m_ViewportSize; }

    void SetViewportMousePos(Vec2 pos) { m_ViewportMousePos = pos; }
    Vec2 GetViewportMousePos() const { return m_ViewportMousePos; }

private:
    void SetDarkThemeColors();
    void SetImGuizmoStyle();

public:
    void init();
    void tick();
    void render();
};
