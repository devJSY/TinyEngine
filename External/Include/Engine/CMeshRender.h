#pragma once
#include "CRenderComponent.h"

class CMeshRender : public CRenderComponent
{
private:
    bool m_DrawAsWire;

public:
    void SetDrawAsWireFrame(bool _b) { m_DrawAsWire = _b; }
    bool GetDrawAsWireFrame() const { return m_DrawAsWire; }

public:
    virtual void UpdateData() override;
    virtual void render() override;

public:
    CMeshRender();
    virtual ~CMeshRender();
};
