#pragma once
#include "CEditor.h"

class CSpriteEditor : public CEditor
{
private:
    Ptr<CTexture> m_pAtlas;

public:
    virtual void init() override;
    virtual void tick() override{};
    virtual void finaltick() override{};
    void render(bool* open);

private:
    virtual void render() override;

public:
    CSpriteEditor();
    virtual ~CSpriteEditor();
};
