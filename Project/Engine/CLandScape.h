#pragma once
#include "CRenderComponent.h"

#include "CHeightMapShader.h"
#include "CRaycastShader.h"

class CLandScape : public CRenderComponent
{
private:
    UINT m_FaceX;
    UINT m_FaceZ;

    Ptr<CHeightMapShader> m_CSHeightMap; // 높이맵 쉐이더
    Ptr<CTexture> m_HeightMapTex;        // 높이맵

    Ptr<CRaycastShader> m_CSRaycast;  // 픽킹 쉐이더
    CStructuredBuffer* m_CrossBuffer; // 마우스 피킹되는 지점 정보 받는 버퍼

    bool m_bDrawLandScape;

    Ptr<CTexture> m_BrushTex; // 브러쉬용 텍스쳐
    float m_BrushStrength;    // 브러쉬 강도
    Vec2 m_BrushScale;        // 브러쉬 크기(전체 지형대비 크기 비율값)

public:
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMapTex = _HeightMap; }

    bool IsDrawLandScape() const { return m_bDrawLandScape; }
    void SetDrawLandScape(bool _bDraw) { m_bDrawLandScape = _bDraw; }

    Ptr<CTexture> GetBrushTex() const { return m_BrushTex; }
    void SetBrushTex(Ptr<CTexture> _Tex) { m_BrushTex = _Tex; }

    float GetBrushStrength() const { return m_BrushStrength; }
    void SetBrushStrength(float _Strength) { m_BrushStrength = _Strength; }

    Vec2 GetBrushScale() const { return m_BrushScale; }
    void SetBrushScale(Vec2 _Scale) { m_BrushScale = _Scale; }

private:
    void Init();
    void CreateMesh();
    void CreateComputeShader();
    void CreateTexture();

public:
    void Raycasting();

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;
    virtual void render(Ptr<CMaterial> _mtrl) override;

public:
    CLONE(CLandScape);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLandScape();
    virtual ~CLandScape();
};
