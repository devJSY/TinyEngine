#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"

#include "CGameObject.h"
#include "CTransform.h"

#include "CConstBuffer.h"
#include "CDevice.h"

CMeshRender::CMeshRender()
    : CRenderComponent(COMPONENT_TYPE::MESHRENDER)
    , m_DrawAsWire(false)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::UpdateData()
{
    if (nullptr != GetShader())
    {
        RS_TYPE RStype = GetShader()->GetRSType();
        if (m_DrawAsWire)
            GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);

        GetShader()->UpdateData();

        GetShader()->SetRSType(RStype);
    }

    if (nullptr != GetMaterial())
    {
        GetMaterial()->UpdateData();
    }

    GetOwner()->Transform()->UpdateData();

    CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL_DATA);
    pCB->SetData(&g_Global);
    pCB->UpdateData(2);
}

void CMeshRender::render()
{
    if (nullptr == GetMesh() || nullptr == GetShader())
        return;

    // Material ÃÊ±âÈ­
    CMaterial::Clear();

    UpdateData();

    GetMesh()->render();
}
