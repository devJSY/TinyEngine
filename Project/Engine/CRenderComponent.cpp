#include "pch.h"
#include "CRenderComponent.h"

#include "CLevelMgr.h"

#include "CLevel.h"
#include "CTransform.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
    : CComponent(_Type)
    , m_Mesh(nullptr)
    , m_SharedMtrl(nullptr)
    , m_DynamicMtrl(nullptr)
    , m_CurMtrl(nullptr)
    , m_bFrustumCheck(true)
    , m_BoundingRadius(1.f)
    , m_bCastShadow(true)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& origin)
    : CComponent(origin)
    , m_Mesh(origin.m_Mesh)
    , m_SharedMtrl(origin.m_SharedMtrl)
    , m_bFrustumCheck(origin.m_bFrustumCheck)
    , m_BoundingRadius(origin.m_BoundingRadius)
    , m_bCastShadow(origin.m_bCastShadow)
{
    // 원본 오브젝트가 공유재질을 참조하고 있고, 현재 사용재질은 공유재질이 아닌경우
    if (origin.m_SharedMtrl != origin.m_CurMtrl)
    {
        assert(origin.m_DynamicMtrl.Get());

        m_CurMtrl = m_DynamicMtrl = origin.m_DynamicMtrl->Clone();
    }
    else
    {
        m_CurMtrl = m_SharedMtrl;
    }
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
    // 재질이 변경되면 기존에 복사본 받아둔 DynamicMaterial 을 삭제한다.
    m_CurMtrl = m_SharedMtrl = _Mtrl;
    m_DynamicMtrl = nullptr;
}

Ptr<CMaterial> CRenderComponent::CreateDynamicMaterial()
{
    //// 런타임 에만 동적 재질 생성
    // CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    // if (!(pCurLevel->GetState() == LEVEL_STATE::PLAY || pCurLevel->GetState() == LEVEL_STATE::SIMULATE))
    //     return nullptr;

    // 이미 동적재질을 보유한경우 리턴
    if (nullptr != m_DynamicMtrl)
        return m_DynamicMtrl;

    // 공유재질이 있는 경우에만 생성
    if (nullptr != m_SharedMtrl)
    {
        // 공유재질을 복사해서 동적재질을 만들고 그걸 현재 사용재질로 설정한다.
        m_CurMtrl = m_DynamicMtrl = m_SharedMtrl->Clone();
        m_DynamicMtrl->SetName(m_DynamicMtrl->GetName() + L"_Inst");
        return m_DynamicMtrl;
    }

    return nullptr;
}

void CRenderComponent::RestoreMaterial()
{
    m_CurMtrl = m_SharedMtrl;
    m_DynamicMtrl = nullptr;
}

void CRenderComponent::finaltick()
{
    if (m_bFrustumCheck)
    {
        GamePlayStatic::DrawDebugSphere(Transform()->GetWorldPos(), m_BoundingRadius, Vec3(1.f, 1.f, 0.f), true);
    }
}

void CRenderComponent::SaveToLevelFile(FILE* _File)
{
    SaveAssetRef<CMesh>(m_Mesh.Get(), _File);
    SaveAssetRef<CMaterial>(m_SharedMtrl.Get(), _File);
    fwrite(&m_bFrustumCheck, 1, sizeof(bool), _File);
    fwrite(&m_BoundingRadius, 1, sizeof(float), _File);
    fwrite(&m_bCastShadow, 1, sizeof(bool), _File);
}

void CRenderComponent::LoadFromLevelFile(FILE* _File)
{
    LoadAssetRef<CMesh>(m_Mesh, _File);
    LoadAssetRef<CMaterial>(m_SharedMtrl, _File);
    fread(&m_bFrustumCheck, 1, sizeof(bool), _File);
    fread(&m_BoundingRadius, 1, sizeof(float), _File);
    fread(&m_bCastShadow, 1, sizeof(bool), _File);

    SetMaterial(m_SharedMtrl);
}
