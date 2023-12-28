#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CRenderComponent : public CComponent
{
private:
    Ptr<CMesh> m_Mesh;
    Ptr<CMaterial> m_Material;

public:
    Ptr<CMesh> GetMesh() const { return m_Mesh; }
    void SetMesh(Ptr<CMesh> _Mesh) { m_Mesh = _Mesh; }

    Ptr<CMaterial> GetMaterial() const { return m_Material; }
    void SetMaterial(Ptr<CMaterial> _Material) { m_Material = _Material; }

public:
    virtual void finaltick(){};
    virtual void UpdateData() = 0;
    virtual void render() = 0;

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    virtual ~CRenderComponent();
};
