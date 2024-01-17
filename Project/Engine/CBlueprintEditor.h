#pragma once
#include "CEditor.h"

namespace ed = ax::NodeEditor;

class CBlueprintEditor : public CEditor
{
private:
    ed::EditorContext* m_Context;

public:
    virtual void init() override;
    virtual void tick() override{};
    virtual void finaltick() override{};
    virtual void render() override;

public:
    CBlueprintEditor();
    virtual ~CBlueprintEditor();
};
