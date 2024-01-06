#pragma once
#include "CAsset.h"

#include "CTexture.h"
#include "CGraphicsShader.h"

class CMaterial : public CAsset
{
public:
    tMtrlConst m_Const;
    Ptr<CTexture> m_arrTex[(UINT)TEX_PARAM::END];
    Ptr<CGraphicsShader> m_pShader;

public:
    void SetShader(Ptr<CGraphicsShader> _Shader) { m_pShader = _Shader; }
    Ptr<CGraphicsShader> GetShader() const { return m_pShader; }

    void SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _Tex) { m_arrTex[(UINT)_Param] = _Tex; }
    Ptr<CTexture> GetTexParam(TEX_PARAM _param) const { return m_arrTex[(UINT)_param]; }

    void GetScalarParam(SCALAR_PARAM _param, void* _pData) const;
    void SetScalarParam(SCALAR_PARAM _Param, const void* _Src);

    void SetMaterialCoefficient(Vec4 _vAmb, Vec4 _vDiff, Vec4 _vSpec, Vec4 _vEmis)
    {
        m_Const.mtrl.vDiff = _vDiff;
        m_Const.mtrl.vAmb = _vAmb;
        m_Const.mtrl.vSpec = _vSpec;
        m_Const.mtrl.vEmv = _vEmis;
    }

public:
    void UpdateData();

public:
    const tMtrlConst& GetMtrlConst() const { return m_Const; }

public:
    CMaterial();
    ~CMaterial();
};
