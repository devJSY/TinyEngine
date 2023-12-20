#pragma once
#include "CEditor.h"

class CLevelEditor : public CEditor
{
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void finaltick() override;
    virtual void render() override;

private:
    void SetDarkThemeColors();

public:
    CLevelEditor();
    virtual ~CLevelEditor();
};
