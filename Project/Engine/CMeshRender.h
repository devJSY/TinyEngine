#pragma once
#include "CRenderComponent.h"

class CMeshRender : public CRenderComponent
{
private:
    bool m_bDrawNormalLine;
    float m_NormalLineScale;
    bool m_bUseTexture;

    bool m_bUseRim;
    Vec3 m_RimColor;
    float m_RimPower;

public:
    void SetDrawNormalLine(bool _b) { m_bDrawNormalLine = _b; }
    bool IsDrawNormalLine() const { return m_bDrawNormalLine; }

    void SetNormalLineScale(float scale) { m_NormalLineScale = scale; }
    float GetNormalLineScale() const { return m_NormalLineScale; }

    void SetUseTexture(bool _b) { m_bUseTexture = _b; }
    bool IsUseTexture() const { return m_bUseTexture; }

    void SetUseRim(bool _b) { m_bUseRim = _b; }
    bool IsUseRim() const { return m_bUseRim; }

    void SetRimColor(Vec3 color) { m_RimColor = color; }
    Vec3 GetRimColor() const { return m_RimColor; }

    void SetRimPower(float _f) { m_RimPower = _f; }
    float GetRimPower() const { return m_RimPower; }

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;

public:
    CMeshRender();
    virtual ~CMeshRender();
};
