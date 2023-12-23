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

void CMaterial::Create(tMaterialData& data)
{
    m_MaterialData = data;
}

void CMaterial::UpdateData()
{
    // Constant Update
    CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
    pCB->SetData(&m_MaterialData);
    pCB->UpdateData(1);
}

void CMaterial::Clear()
{
    ID3D11Buffer* pBuffer = nullptr;
    CONTEXT->VSSetConstantBuffers(1, 1, &pBuffer);
    CONTEXT->HSSetConstantBuffers(1, 1, &pBuffer);
    CONTEXT->DSSetConstantBuffers(1, 1, &pBuffer);
    CONTEXT->GSSetConstantBuffers(1, 1, &pBuffer);
    CONTEXT->PSSetConstantBuffers(1, 1, &pBuffer);
}
