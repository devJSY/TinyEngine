#include "pch.h"
#include "CTileMap.h"

#include "CStructuredBuffer.h"

#include "CAssetMgr.h"
#include "CMesh.h"

#include "CTransform.h"

CTileMap::CTileMap()
    : CRenderComponent(COMPONENT_TYPE::TILEMAP)
    , m_iTileCountX(0)
    , m_iTileCountY(0)
    , m_vTileRenderSize(Vec2(64.f, 64.f))
    , m_TileAtlas()
    , m_vTilePixelSize()
    , m_vSliceSizeUV()
    , m_vecTileInfo{}
    , m_TileInfoBuffer(nullptr)
{
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileMapMtrl"));

    m_TileInfoBuffer = new CStructuredBuffer;

    SetTileCount(m_iTileCountX, m_iTileCountY);
}

CTileMap::~CTileMap()
{
    if (nullptr != m_TileInfoBuffer)
        delete m_TileInfoBuffer;
}

void CTileMap::finaltick()
{
    // (타일 개수 * 타일 사이즈) 로 사이즈를 변경처리한다.
    Vec3 vTileMapSize = Vec3(m_iTileCountX * m_vTileRenderSize.x, m_iTileCountY * m_vTileRenderSize.y, 1.f);
    Transform()->SetRelativeScale(vTileMapSize);
}

void CTileMap::render()
{
    // 재질에 아틀라스 텍스쳐 전달.
    GetMaterial()->SetTexParam(TEX_0, m_TileAtlas);

    // 타일의 가로 세로 개수
    GetMaterial()->SetScalarParam(INT_0, &m_iTileCountX);
    GetMaterial()->SetScalarParam(INT_1, &m_iTileCountY);

    // 아틀라스 이미지에서 타일 1개의 자르는 사이즈(UV 기준)
    GetMaterial()->SetScalarParam(VEC2_0, &m_vSliceSizeUV);

    // 각 타일 정보를 구조화 버퍼로 이동
    m_TileInfoBuffer->SetData(m_vecTileInfo.data(), (UINT)m_vecTileInfo.size());

    // 타일 구조화 버퍼를 t20 에 바인딩
    m_TileInfoBuffer->UpdateData(20);

    // 재질 업데이트
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

    m_vSliceSizeUV = Vec2(m_vTilePixelSize.x / m_TileAtlas->GetWidth(), m_vTilePixelSize.y / m_TileAtlas->GetHeight());
}

void CTileMap::SetTileCount(UINT _TileCountX, UINT _TileCountY)
{
    m_iTileCountX = _TileCountX;
    m_iTileCountY = _TileCountY;

    vector<tTileInfo> vecTemp;
    m_vecTileInfo.swap(vecTemp);
    m_vecTileInfo.resize(m_iTileCountX * m_iTileCountY);

    m_TileInfoBuffer->Create(sizeof(tTileInfo), m_iTileCountX * m_iTileCountY, SB_TYPE::READ_ONLY, true);
}

void CTileMap::SetTileIndex(UINT _Row, UINT _Col, UINT _ImgIdx)
{
    if (nullptr == m_TileAtlas)
        return;

    UINT idx = _Row * m_iTileCountX + _Col;

    // 렌더링할 타일 정보
    UINT iRow = _ImgIdx / UINT(m_vTilePixelSize.x * m_iTileCountX);
    UINT iCol = _ImgIdx % UINT(m_vTilePixelSize.y * m_iTileCountX);

    m_vecTileInfo[idx].vLeftTopUV = Vec2((iCol * m_vTilePixelSize.x) / m_TileAtlas->GetWidth(),
                                         (iRow * m_vTilePixelSize.y) / m_TileAtlas->GetHeight());

    m_vecTileInfo[idx].bRender = 1;
}

void CTileMap::SaveToLevelFile(FILE* _File)
{
}

void CTileMap::LoadFromLevelFile(FILE* _File)
{
}
