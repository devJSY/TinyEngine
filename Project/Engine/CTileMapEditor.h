#pragma once
#include "CEditor.h"

class CTileMapEditor : public CEditor
{
public:
    virtual void init() override{};
    virtual void tick() override{};
    virtual void finaltick() override{};
    void render(bool* open);

private:
    virtual void render() override;

public:
    CTileMapEditor();
    virtual ~CTileMapEditor();
};
