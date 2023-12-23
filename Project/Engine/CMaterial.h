#pragma once
#include "CAsset.h"

class CMaterial : public CAsset
{
private:
    tMaterial m_material;

public:
    void Create(tMaterial& _material);
    void UpdateData();

public:
    CMaterial();
    virtual ~CMaterial();
};
