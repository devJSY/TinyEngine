#include "pch.h"
#include "CLandScape.h"

#include "CTransform.h"

CLandScape::CLandScape()
    : CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
    , m_FaceX(64)
    , m_FaceZ(64)
{
    Init();
}

CLandScape::~CLandScape()
{
}

void CLandScape::finaltick()
{
}

void CLandScape::render()
{
    UpdateData();

    GetMesh()->render();
}

void CLandScape::render(Ptr<CMaterial> _mtrl)
{
}

void CLandScape::UpdateData()
{
    Transform()->UpdateData();

    GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, m_FaceX);
    GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, m_FaceZ);
    GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HeightMapTex);

    GetMaterial()->UpdateData();
}

void CLandScape::SaveToLevelFile(FILE* _File)
{
    CRenderComponent::SaveToLevelFile(_File);
}

void CLandScape::LoadFromLevelFile(FILE* _File)
{
    CRenderComponent::LoadFromLevelFile(_File);
}
