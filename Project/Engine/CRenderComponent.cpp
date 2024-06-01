#include "pch.h"
#include "CRenderComponent.h"

#include "CLevelMgr.h"

#include "CLevel.h"
#include "CTransform.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
    : CComponent(_Type)
    , m_Mesh(nullptr)
    , m_vecMtrls{}
    , m_bFrustumCheck(true)
    , m_BoundingRadius(1.f)
    , m_bCastShadow(true)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& origin)
    : CComponent(origin)
    , m_Mesh(origin.m_Mesh)
    , m_vecMtrls{}
    , m_bFrustumCheck(origin.m_bFrustumCheck)
    , m_BoundingRadius(origin.m_BoundingRadius)
    , m_bCastShadow(origin.m_bCastShadow)
{
    m_vecMtrls.resize(origin.m_vecMtrls.size());

    for (UINT i = 0; i < (UINT)origin.m_vecMtrls.size(); ++i)
    {
        m_vecMtrls[i].pCurMtrl = origin.m_vecMtrls[i].pCurMtrl;
        m_vecMtrls[i].pSharedMtrl = origin.m_vecMtrls[i].pSharedMtrl;

        // 원본 오브젝트가 공유재질을 참조하고 있고, 현재 사용재질은 공유재질이 아닌경우
        if (origin.m_vecMtrls[i].pSharedMtrl != origin.m_vecMtrls[i].pCurMtrl)
        {
            assert(origin.m_vecMtrls[i].pDynamicMtrl.Get());

            // 복사 렌더 컴포넌트도 별도의 동적재질을 생성한다.
            GetDynamicMaterial(i);

            // 원본 렌더컴포넌트의 동적재질 값을 현재 생성한 동적재질로 복사한다.
            m_vecMtrls[i].pDynamicMtrl = origin.m_vecMtrls[i].pDynamicMtrl->Clone();
        }
        else
        {
            m_vecMtrls[i].pCurMtrl = m_vecMtrls[i].pSharedMtrl;
        }
    }
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMesh(Ptr<CMesh> _Mesh)
{
    m_Mesh = _Mesh;

    if (!m_vecMtrls.empty())
    {
        m_vecMtrls.clear();
        vector<tMtrlSet> vecMtrls;
        m_vecMtrls.swap(vecMtrls);
    }

    if (nullptr != m_Mesh)
    {
        m_vecMtrls.resize(m_Mesh->GetSubsetCount());
    }
}

Ptr<CMaterial> CRenderComponent::GetMaterial(UINT _idx)
{
    return m_vecMtrls[_idx].pCurMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial(UINT _idx)
{
    // 공유재질을 가져오는것으로 현재 사용재질을 동적재질에서 회복하도록 한다
    m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pSharedMtrl;

    if (m_vecMtrls[_idx].pDynamicMtrl.Get())
    {
        m_vecMtrls[_idx].pDynamicMtrl = nullptr;
    }

    return m_vecMtrls[_idx].pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial(UINT _idx)
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    if (pCurLevel->GetState() != LEVEL_STATE::PLAY)
        return nullptr;

    // 원본 재질이 없다 -> Nullptr 반환
    if (nullptr == m_vecMtrls[_idx].pSharedMtrl)
    {
        m_vecMtrls[_idx].pCurMtrl = nullptr;
        m_vecMtrls[_idx].pDynamicMtrl = nullptr;
        return m_vecMtrls[_idx].pCurMtrl;
    }

    if (nullptr == m_vecMtrls[_idx].pDynamicMtrl)
    {
        m_vecMtrls[_idx].pDynamicMtrl = m_vecMtrls[_idx].pSharedMtrl->Clone();
        m_vecMtrls[_idx].pDynamicMtrl->SetName(m_vecMtrls[_idx].pSharedMtrl->GetName() + L"_Clone");
        m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pDynamicMtrl;
    }

    return m_vecMtrls[_idx].pCurMtrl;
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx)
{
    // 재질이 변경되면 기존에 복사본 받아둔 DynamicMaterial 을 삭제한다.
    m_vecMtrls[_idx].pSharedMtrl = _Mtrl;
    m_vecMtrls[_idx].pCurMtrl = _Mtrl;
    m_vecMtrls[_idx].pDynamicMtrl = nullptr;
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

    // 재질 참조정보 저장
    UINT iMtrlCount = GetMtrlCount();
    fwrite(&iMtrlCount, sizeof(UINT), 1, _File);

    for (UINT i = 0; i < iMtrlCount; ++i)
    {
        SaveAssetRef(m_vecMtrls[i].pSharedMtrl, _File);
    }

    fwrite(&m_bFrustumCheck, 1, sizeof(bool), _File);
    fwrite(&m_BoundingRadius, 1, sizeof(float), _File);
    fwrite(&m_bCastShadow, 1, sizeof(bool), _File);
}

void CRenderComponent::LoadFromLevelFile(FILE* _File)
{
    LoadAssetRef<CMesh>(m_Mesh, _File);

    // 재질 참조정보 불러오기
    UINT iMtrlCount = GetMtrlCount();
    fread(&iMtrlCount, sizeof(UINT), 1, _File);
    m_vecMtrls.resize(iMtrlCount);
    for (UINT i = 0; i < iMtrlCount; ++i)
    {
        LoadAssetRef(m_vecMtrls[i].pSharedMtrl, _File);
    }

    fread(&m_bFrustumCheck, 1, sizeof(bool), _File);
    fread(&m_BoundingRadius, 1, sizeof(float), _File);
    fread(&m_bCastShadow, 1, sizeof(bool), _File);
}
