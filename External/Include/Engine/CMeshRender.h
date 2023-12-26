#pragma once
#include "CRenderComponent.h"

class CMeshRender : public CRenderComponent
{
private:
    CGraphicsShader* m_NormalLineShader;
    bool m_bDrawNormalLine;
    float m_NormalLineScale;
    bool m_bDrawAsWire;
    bool m_bUseTexture;

    bool m_bUseRim;
    Vec3 m_RimColor;
    float m_RimPower;

public:
    void SetNormalLineShader(CGraphicsShader* shader) { m_NormalLineShader = shader; }

    void SetDrawNormalLine(bool _b) { m_bDrawNormalLine = _b; }
    bool IsDrawNormalLine() const { return m_bDrawNormalLine; }

    void SetNormalLineScale(float scale) { m_NormalLineScale = scale; }
    float GetNormalLineScale() const { return m_NormalLineScale; }

    void SetDrawAsWireFrame(bool _b) { m_bDrawAsWire = _b; }
    bool IsDrawAsWireFrame() const { return m_bDrawAsWire; }

    void SetUseTexture(bool _b) { m_bUseTexture = _b; }
    bool IsUseTexture() const { return m_bUseTexture; }

    // Rim
    void SetUseRim(bool _b) { m_bUseRim = _b; }
    bool IsUseRim() const { return m_bUseRim; }

    void SetRimColor(Vec3 color) { m_RimColor = color; }
    Vec3 GetRimColor() const { return m_RimColor; }

    void SetRimPower(float _f) { m_RimPower = _f; }
    float GetRimPower() const { return m_RimPower; }

public:
    virtual void UpdateData() override;
    virtual void render() override;

public:
    CMeshRender();
    virtual ~CMeshRender();
};
