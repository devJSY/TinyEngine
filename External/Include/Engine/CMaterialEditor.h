#pragma once
#include "CEditor.h"

class CMaterialEditor : public CEditor
{
private:
    Ptr<CMaterial> m_Mtrl;

public:
    virtual void init() override{};
    virtual void tick() override{};
    virtual void finaltick() override{};
    void render(bool* open);

private:
    virtual void render() override;

private:
    void DrawDetails();

public:
    void SetMaterial(Ptr<CMaterial> _Mtrl) { m_Mtrl = _Mtrl; }

public:
    CMaterialEditor();
    virtual ~CMaterialEditor();
};
