#include "pch.h"
#include "CAsset.h"

CAsset::CAsset(ASSET_TYPE _Type)
    : m_Type(_Type)
    , m_RefCount(0)
{
}

CAsset::~CAsset()
{
}
