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
    for (UINT i = TEX_PARAM::TEX_0; i < TEX_PARAM::END; ++i)
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

void CMaterial::GetScalarParam(SCALAR_PARAM _param, void* _pData) 
{
    switch (_param)
    {
    case INT_0:
    case INT_1:
    case INT_2:
    case INT_3:
        {
            int idx = (UINT)_param - (UINT)INT_0;
            *((int*)_pData) = m_Const.arrInt[idx];
        }
        break;
    case FLOAT_0:
    case FLOAT_1:
    case FLOAT_2:
    case FLOAT_3:
        {
            int idx = (UINT)_param - (UINT)FLOAT_0;
            *((float*)_pData) = m_Const.arrFloat[idx];
        }
        break;

    case VEC2_0:
    case VEC2_1:
    case VEC2_2:
    case VEC2_3:
        {
            int idx = (UINT)_param - (UINT)VEC2_0;
            *((Vec2*)_pData) = m_Const.arrVec2[idx];
        }
        break;

    case VEC4_0:
    case VEC4_1:
    case VEC4_2:
    case VEC4_3:
        {
            int idx = (UINT)_param - (UINT)VEC4_0;
            *((Vec4*)_pData) = m_Const.arrVec4[idx];
        }
        break;

    case MAT_0:
    case MAT_1:
    case MAT_2:
    case MAT_3:
        {
            int idx = (UINT)_param - (UINT)MAT_0;
            *((Matrix*)_pData) = m_Const.arrMat[idx];
        }
        break;
    }
}

void CMaterial::SetScalarParam(SCALAR_PARAM _Param, const void* _Src)
{
    switch (_Param)
    {
    case INT_0:
    case INT_1:
    case INT_2:
    case INT_3:
        m_Const.arrInt[_Param] = *((int*)_Src);
        break;
    case FLOAT_0:
    case FLOAT_1:
    case FLOAT_2:
    case FLOAT_3:
        m_Const.arrFloat[_Param - FLOAT_0] = *((float*)_Src);
        break;

    case VEC2_0:
    case VEC2_1:
    case VEC2_2:
    case VEC2_3:
        m_Const.arrVec2[_Param - VEC2_0] = *((Vec2*)_Src);
        break;

    case VEC4_0:
    case VEC4_1:
    case VEC4_2:
    case VEC4_3:
        m_Const.arrVec4[_Param - VEC4_0] = *((Vec4*)_Src);
        break;

    case MAT_0:
    case MAT_1:
    case MAT_2:
    case MAT_3:
        m_Const.arrMat[_Param - MAT_0] = *((Matrix*)_Src);
        break;
    }
}
