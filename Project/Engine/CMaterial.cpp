#include "pch.h"
#include "CMaterial.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CMaterial::CMaterial()
    : CAsset(ASSET_TYPE::MATERIAL)
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::Create(tMaterial& _material)
{
    m_material = _material;
}

void CMaterial::UpdateData()
{
    // Constant Update
    CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
    pCB->SetData(&m_material);
    pCB->UpdateData(1);
}
