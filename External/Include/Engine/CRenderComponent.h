#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CRenderComponent : public CComponent
{
private:
    Ptr<CMesh> m_Mesh;

    Ptr<CMaterial> m_CurMtrl;
    Ptr<CMaterial> m_SharedMtrl;
    Ptr<CMaterial> m_DynamicMtrl;

    bool m_bCastShadow;

public:
    Ptr<CMesh> GetMesh() const { return m_Mesh; }
    void SetMesh(Ptr<CMesh> _Mesh) { m_Mesh = _Mesh; }

    Ptr<CMaterial> GetMaterial() const { return m_CurMtrl; }
    Ptr<CMaterial> GetSharedMaterial() const { return m_SharedMtrl; }
    Ptr<CMaterial> GetDynamicMaterial() const { return m_DynamicMtrl; };
    Ptr<CMaterial> CreateDynamicMaterial();
    void SetMaterial(Ptr<CMaterial> _Mtrl);

    void RestoreMaterial();

    bool IsCastShadow() const { return m_bCastShadow; }
    void SetCastShadow(bool _bCastShadow) { m_bCastShadow = _bCastShadow; }

public:
    virtual void finaltick(){};
    virtual void UpdateData() = 0;
    virtual void render() = 0;
    virtual void render(Ptr<CMaterial> _mtrl) = 0;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual CRenderComponent* Clone() = 0;

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& origin);
    virtual ~CRenderComponent();
};
