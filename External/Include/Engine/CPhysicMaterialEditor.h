#pragma once
#include "CEditor.h"

class CPhysicMaterialEditor : public CEditor
{
private:
    Ptr<CPhysicMaterial> m_Mtrl;

public:
    virtual void init() override{};
    virtual void tick() override{};
    virtual void finaltick() override{};
    virtual void render(bool* open) override;

private:
    void render();

private:
    void DrawDetails();

public:
    void SetMaterial(Ptr<CPhysicMaterial> _Mtrl) { m_Mtrl = _Mtrl; }

public:
    CPhysicMaterialEditor();
    virtual ~CPhysicMaterialEditor();
};
