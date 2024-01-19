#include "pch.h"
#include "CTileMap.h"

#include "CAssetMgr.h"
#include "CMesh.h"

#include "CTransform.h"

CTileMap::CTileMap()
    : CRenderComponent(COMPONENT_TYPE::TILEMAP)
    , m_iTileCountX(1)
    , m_iTileCountY(1)
    , m_vTileRenderSize(Vec2(128.f, 128.f))
    , m_MaxCol(0)
    , m_MaxRow(0)
    , m_TileIdx(3)
{
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileMapMtrl"));
}

CTileMap::~CTileMap()
{
}

void CTileMap::finaltick()
{
    // (타일 개수 * 타일 사이즈) 로 사이즈를 변경처리한다.
    Vec3 vTileMapSize = Vec3(m_iTileCountX * m_vTileRenderSize.x, m_iTileCountY * m_vTileRenderSize.y, 1.f);
    Transform()->SetRelativeScale(vTileMapSize);
}

void CTileMap::render()
{
    if (nullptr == m_TileAtlas)
        return;

    // 재질에 아틀라스 텍스쳐 전달.
    GetMaterial()->SetTexParam(TEX_0, m_TileAtlas);

    // 렌더링할 타일 정보
    UINT iRow = m_TileIdx / m_MaxCol;
    UINT iCol = m_TileIdx % m_MaxCol;

    Vec2 vLeftTopUV = Vec2((iCol * m_vTilePixelSize.x) / m_TileAtlas->GetWidth(),
                           (iRow * m_vTilePixelSize.y) / m_TileAtlas->GetHeight());

    Vec2 vSliceSizeUV =
        Vec2(m_vTilePixelSize.x / m_TileAtlas->GetWidth(), m_vTilePixelSize.y / m_TileAtlas->GetHeight());

    GetMaterial()->SetScalarParam(VEC2_0, &vLeftTopUV);
    GetMaterial()->SetScalarParam(VEC2_1, &vSliceSizeUV);

    GetMaterial()->UpdateData();

    Transform()->UpdateData();

    GetMesh()->render();
}

void CTileMap::UpdateData()
{
}

void CTileMap::SetTileAtlas(Ptr<CTexture> _Atlas, Vec2 _TilePixelSize)
{
    m_TileAtlas = _Atlas;
    m_vTilePixelSize = _TilePixelSize;

    m_MaxCol = m_TileAtlas->GetWidth() / (UINT)m_vTilePixelSize.x;
    m_MaxRow = m_TileAtlas->GetHeight() / (UINT)m_vTilePixelSize.y;
}

void CTileMap::SaveToLevelFile(FILE* _File)
{
}

void CTileMap::LoadFromLevelFile(FILE* _File)
{
}
