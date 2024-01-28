#pragma once

#include "CRenderComponent.h"

class CStructuredBuffer;

struct tTileInfo
{
    Vec2 vLeftTopUV = Vec2();
    int bRender = false;
    int ImageIndex = -1;
};

class CTileMap : public CRenderComponent
{
private:
    UINT m_iTileCountX;     // 가로 타일 개수
    UINT m_iTileCountY;     // 세로 타일 개수
    Vec2 m_vTileRenderSize; // 레벨 내에서의 렌더링할 타일 사이즈

    Ptr<CTexture> m_TileAtlas;
    Vec2 m_vTilePixelSize; // 파일 내에서의 타일 사이즈
    Vec2 m_vSliceSizeUV;

    vector<tTileInfo> m_vecTileInfo;
    CStructuredBuffer* m_TileInfoBuffer;

public:
    UINT GetTileCountX() const { return m_iTileCountX; }
    UINT GetTileCountY() const { return m_iTileCountY; }
    void SetTileCount(UINT _TileCountX, UINT _TileCountY);

    Vec2 GetTileRenderSize() const { return m_vTileRenderSize; }
    void SetTileRenderSize(Vec2 _size) { m_vTileRenderSize = _size; }

    void SetTileAtlas(Ptr<CTexture> _Atlas, Vec2 _TilePixelSize);
    Ptr<CTexture> GetTileAtlas() const { return m_TileAtlas; }

    Vec2 GetTilePixelSize() const { return m_vTilePixelSize; }

    void SetTileIndex(UINT _Row, UINT _Col, UINT _ImgIdx); // row,col 위치의 타일에 ImgIdx를 배정
public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CTileMap();
    virtual ~CTileMap();

    friend class CTileMapEditor;
};
