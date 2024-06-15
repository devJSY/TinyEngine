#pragma once
#include "CRenderComponent.h"

#include "CHeightMapShader.h"
#include "CRaycastShader.h"
#include "CWeightMapShader.h"

enum class LANDSCAPE_MODE
{
    HEIGHT_MAP,
    SPLAT,
    NONE,
};

struct tWeight_4
{
    float arrWeight[4];
};

class CLandScape : public CRenderComponent
{
private:
    UINT m_FaceX;
    UINT m_FaceZ;

    Ptr<CHeightMapShader> m_CSHeightMap; // 높이맵 쉐이더
    Ptr<CTexture> m_HeightMapTex;        // 높이맵

    Ptr<CRaycastShader> m_CSRaycast;  // 픽킹 쉐이더
    CStructuredBuffer* m_CrossBuffer; // 마우스 피킹되는 지점 정보 받는 버퍼

    Ptr<CWeightMapShader> m_CSWeightMap;  // 가중치 쉐이더
    CStructuredBuffer* m_WeightMapBuffer; // 가중치 저장 버퍼
    UINT m_WeightWidth;                   // 가중치 버퍼 가로세로 행렬 수
    UINT m_WeightHeight;                  // 가중치 버퍼 가로세로 행렬 수
    UINT m_WeightIdx;                     // 증가 시킬 가중치 부위

    Ptr<CTexture> m_TileArrTex; // 타일 배열 텍스쳐

    LANDSCAPE_MODE m_Mode;

    Ptr<CTexture> m_BrushTex; // 브러쉬용 텍스쳐
    float m_BrushStrength;    // 브러쉬 강도
    Vec2 m_BrushScale;        // 브러쉬 크기(전체 지형대비 크기 비율값)

public:
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMapTex = _HeightMap; }

    LANDSCAPE_MODE GetLandScapeMode() const { return m_Mode; }
    void SetLandScapeMode(LANDSCAPE_MODE _Mode) { m_Mode = _Mode; }

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
    virtual void render(UINT _Subset) override;

public:
    CLONE(CLandScape);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLandScape();
    virtual ~CLandScape();
};
