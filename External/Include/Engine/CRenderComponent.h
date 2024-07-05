#pragma once

#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CMeshData.h"

struct tMtrlSet
{
    Ptr<CMaterial> pSharedMtrl;  // 공유 메테리얼
    Ptr<CMaterial> pDynamicMtrl; // 공유 메테리얼의 복사본
    Ptr<CMaterial> pCurMtrl;     // 현재 사용 할 메테리얼
};

class CRenderComponent : public CComponent
{
private:
    Ptr<CMesh> m_Mesh;
    vector<tMtrlSet> m_vecMtrls; // 재질

    bool m_bFrustumCheck;
    float m_BoundingRadius;
    bool m_bCastShadow;

public:
    Ptr<CMesh> GetMesh() const { return m_Mesh; }
    void SetMesh(Ptr<CMesh> _Mesh);

    Ptr<CMaterial> GetMaterial(UINT _idx);
    Ptr<CMaterial> GetSharedMaterial(UINT _idx);
    Ptr<CMaterial> GetDynamicMaterial(UINT _idx);
    void SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx);

    Ptr<CMaterial> GetMaterial(wstring _Name);
    Ptr<CMaterial> GetSharedMaterial(wstring _Name);
    Ptr<CMaterial> GetDynamicMaterial(wstring _Name);
    void SetMaterial(Ptr<CMaterial> _Mtrl, wstring _Name);

    void SetMeshData(Ptr<CMeshData> _MeshData);

    UINT GetMtrlCount() const { return (UINT)m_vecMtrls.size(); }

    bool IsFrustumCheck() const { return m_bFrustumCheck; }
    void SetFrustumCheck(bool _bCheck) { m_bFrustumCheck = _bCheck; }

    float GetBoundingRadius() const { return m_BoundingRadius; }
    void SetBoundingRadius(float _fRadius) { m_BoundingRadius = _fRadius; }

    bool IsCastShadow() const { return m_bCastShadow; }
    void SetCastShadow(bool _bCastShadow) { m_bCastShadow = _bCastShadow; }

    ULONG64 GetInstID(UINT _iMtrlIdx);

public:
    virtual void finaltick();
    virtual void UpdateData() = 0;
    virtual void render() = 0;
    virtual void render(Ptr<CMaterial> _mtrl) = 0;
    virtual void render(UINT _iSubset) = 0;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual CRenderComponent* Clone() = 0;

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& origin);
    virtual ~CRenderComponent();
};
