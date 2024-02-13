#include "pch.h"
#include "CAsset.h"

CAsset::CAsset(ASSET_TYPE _Type, bool _bEngineAsset)
    : m_Type(_Type)
    , m_RefCount(0)
    , m_bEngineAsset(_bEngineAsset)
{
}

CAsset::~CAsset()
{
}
