#pragma once
#include "CComponent.h"

class CMesh;
class CGraphicsShader;
class CMaterial;

class CRenderComponent : public CComponent
{
private:
    CMesh* m_Mesh;
    CGraphicsShader* m_Shader;

public:
    void SetMesh(CMesh* _Mesh) { m_Mesh = _Mesh; }
    void SetShader(CGraphicsShader* _Shader) { m_Shader = _Shader; }

    CMesh* GetMesh() const { return m_Mesh; }
    CGraphicsShader* GetShader() const { return m_Shader; }

private:
    CMaterial* m_Material;

public:
    void SetMaterial(CMaterial* _Material) { m_Material = _Material; }
    CMaterial* GetMaterial() const { return m_Material; }

public:
    virtual void finaltick(){};
    virtual void UpdateData() = 0;
    virtual void render() = 0;

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    virtual ~CRenderComponent();
};
