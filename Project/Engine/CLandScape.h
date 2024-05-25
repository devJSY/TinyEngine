#pragma once
#include "CRenderComponent.h"

#include "CHeightMapShader.h"
#include "CRaycastShader.h"

class CLandScape : public CRenderComponent
{
private:
    UINT m_FaceX;
    UINT m_FaceZ;

    Vec2 m_BrushScale;        // 브러쉬 크기(전체 지형대비 크기 비율값)
    Ptr<CTexture> m_BrushTex; // 브러쉬용 텍스쳐

    Ptr<CHeightMapShader> m_CSHeightMap; // 높이맵 쉐이더
    Ptr<CTexture> m_HeightMapTex;        // 높이맵

    Ptr<CRaycastShader> m_CSRaycast;  // 픽킹 쉐이더
    CStructuredBuffer* m_CrossBuffer; // 마우스 피킹되는 지점 정보 받는 버퍼

public:
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMapTex = _HeightMap; }

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
