#pragma once
#include "CAsset.h"

class CMaterial : public CAsset
{
private:
    tMaterialData m_MaterialData;

public:
    tMaterialData GetMaterialData() const { return m_MaterialData; }
    void SetMaterialData(tMaterialData data) { m_MaterialData = data; }

public:
    void Create(tMaterialData& data);
    void UpdateData();

    static void Clear();

public:
    CMaterial();
    virtual ~CMaterial();
};
