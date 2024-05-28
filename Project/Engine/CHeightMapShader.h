#pragma once
#include "CComputeShader.h"

#include "ptr.h"
#include "CTexture.h"

class CStructuredBuffer;

class CHeightMapShader : public CComputeShader
{
private:
    Ptr<CTexture> m_pHeightMap;
    Ptr<CTexture> m_pBrushTex;
    Vec2 m_vScale;    // 높이 Brush 크기
    float m_Strength; // 강도
    int m_iBrushIdx;
    CStructuredBuffer* m_pInput; // Ray 충돌 위치

public:
    void SetHeightMap(Ptr<CTexture> _pTex) { m_pHeightMap = _pTex; }
    void SetBrushTex(Ptr<CTexture> _pTex) { m_pBrushTex = _pTex; }
    void SetBrushStrength(float _Strength) { m_Strength = _Strength; }
    void SetBrushScale(Vec2 _vScale) { m_vScale = _vScale; }
    void SetBrushIndex(int _iIdx) { m_iBrushIdx = _iIdx; }
    void SetInputBuffer(CStructuredBuffer* _pInput) { m_pInput = _pInput; }

public:
    virtual int UpdateData() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;

public:
    CHeightMapShader();
    virtual ~CHeightMapShader();
};