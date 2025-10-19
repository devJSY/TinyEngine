#pragma once
#include <Engine/CEntity.h>

// 2025/10/19(а╓╫б©╛): юс╫ц
#include "define.h"

class CEditor : public CEntity
{
private:
    EDITOR_TYPE m_EditorType;

public:
    virtual void init() = 0;
    virtual void tick() = 0;
    virtual void finaltick() = 0;
    virtual void render(bool* open) = 0;

public:
    EDITOR_TYPE GetEditorType() const { return m_EditorType; }

    CLONE_DISABLE(CEditor);

public:
    CEditor(EDITOR_TYPE type);
    virtual ~CEditor();
};
