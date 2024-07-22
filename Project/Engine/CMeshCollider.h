#pragma once
#include "CCollider.h"

class CMeshCollider : public CCollider
{
private:
    bool m_bConvex;
    Ptr<CMesh> m_Mesh;

public:
    virtual void begin() override;
    virtual void finaltick() override;

public:
    bool IsConvex() const { return m_bConvex; }
    void SetConvex(bool _bEnable) { m_bConvex = _bEnable; }

    Ptr<CMesh> GetMesh() const { return m_Mesh; }
    void SetMesh(Ptr<CMesh> _pMesh) { m_Mesh = _pMesh; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CMeshCollider);

public:
    CMeshCollider();
    virtual ~CMeshCollider();

    friend class CPhysicsMgr;
};
