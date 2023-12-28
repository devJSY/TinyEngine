#include "pch.h"
#include "CMaterial.h"

#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTexture.h"

CMaterial::CMaterial()
    : CAsset(ASSET_TYPE::MATERIAL)
    , m_Const{}
    , m_arrTex{}
    , m_pShader(nullptr)
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::UpdateData()
{
    if (nullptr == m_pShader.Get())
        return;

    // 사용할 쉐이더 바인딩
    m_pShader->UpdateData();

    // Texture Update(Register Binding)
    for (UINT i = 0; i < TEX_PARAM::END; ++i)
    {
        if (nullptr != m_arrTex[i].Get())
        {
            m_arrTex[i]->UpdateData(i);
            m_Const.bTex[i] = 1;
        }
        else
        {
            CTexture::Clear(i);
            m_Const.bTex[i] = 0;
        }
    }

    // 상수 데이터 업데이트
    static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
    pCB->SetData(&m_Const);
    pCB->UpdateData();
}