#pragma once
#include "CComponent.h"

class CMesh;
class CGraphicsShader;
class CMaterial;

class CRenderComponent : public CComponent
{
private:
    CMesh* m_Mesh;
    CMaterial* m_Material;

public:
    CMesh* GetMesh() const { return m_Mesh; }
    void SetMesh(CMesh* _Mesh) { m_Mesh = _Mesh; }

    CMaterial* GetMaterial() const { return m_Material; }
    void SetMaterial(CMaterial* _Material) { m_Material = _Material; }

public:
    virtual void finaltick(){};
    virtual void UpdateData() = 0;
    virtual void render() = 0;

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    virtual ~CRenderComponent();
};
