#pragma once
#include "CAsset.h"

class CGraphicsShader;
class CTexture;

class CMaterial : public CAsset
{
public:
    tMtrlConst m_Const;
    CTexture* m_arrTex[(UINT)TEX_PARAM::END];

    CGraphicsShader* m_pShader;

public:
    void SetTexParam(TEX_PARAM _Param, CTexture* _Tex) { m_arrTex[(UINT)_Param] = _Tex; }
    CTexture* GetTexParam(TEX_PARAM _param) const { return m_arrTex[(UINT)_param]; }

    void SetShader(CGraphicsShader* _Shader) { m_pShader = _Shader; }
    CGraphicsShader* GetShader() const { return m_pShader; }

    void SetMaterialCoefficient(Vec4 _vAmb, Vec4 _vDiff, Vec4 _vSpec, Vec4 _vEmis)
    {
        m_Const.mtrl.vDiff = _vDiff;
        m_Const.mtrl.vAmb = _vAmb;
        m_Const.mtrl.vSpec = _vSpec;
        m_Const.mtrl.vEmv = _vEmis;
    }

    void UpdateData();

public:
    CMaterial();
    ~CMaterial();
};
