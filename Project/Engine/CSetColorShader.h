#pragma once
#include "CComputeShader.h"

class CSetColorShader : public CComputeShader
{
private:
    Ptr<CTexture> m_TargetTex;
    Vec4 m_Color;

public:
    void SetTargetTexture(Ptr<CTexture> _tex) { m_TargetTex = _tex; }
    void SetColor(Vec3 _Color) { m_Color = _Color; }

public:
    virtual void UpdateData() override;
    virtual void Clear() override;

public:
    CSetColorShader();
    virtual ~CSetColorShader();
};
