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
    if (nullptr != m_pShader)
    {
        m_pShader->UpdateData();
    }

    // Texture Update
    for (UINT i = 0; i < TEX_PARAM::END; ++i)
    {
        if (nullptr == m_arrTex[i])
        {
            m_Const.arrTex[i] = 0;
            CTexture::Clear(i);
            continue;
        }
        else
        {
            m_Const.arrTex[i] = 1;
            m_arrTex[i]->UpdateData(i);
        }
    }

    static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
    pCB->SetData(&m_Const);
    pCB->UpdateData();
}