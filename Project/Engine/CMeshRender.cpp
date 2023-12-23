#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"

#include "CGameObject.h"
#include "CTransform.h"

CMeshRender::CMeshRender()
    : CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::UpdateData()
{
    if (nullptr != GetShader())
    {
        GetShader()->UpdateData();
    }

    if (nullptr != GetMaterial())
    {
        GetMaterial()->UpdateData();
    }

    GetOwner()->Transform()->UpdateData();
}

void CMeshRender::render()
{
    if (nullptr == GetMesh() || nullptr == GetShader())
        return;

    UpdateData();

    GetMesh()->render();
}
