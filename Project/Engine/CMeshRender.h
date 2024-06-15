#pragma once
#include "CRenderComponent.h"

class CMeshRender : public CRenderComponent
{
public:
    virtual void finaltick() override;
    virtual void UpdateData() override {};
    virtual void render() override;
    virtual void render(Ptr<CMaterial> _mtrl) override;
    virtual void render(UINT _Subset) override;

public:
    CLONE(CMeshRender);

public:
    CMeshRender();
    virtual ~CMeshRender();
};
