#include "pch.h"
#include "CPlayerDamageLifeBarScript.h"
#include "CPlayerScript.h"

CPlayerDamageLifeBarScript::CPlayerDamageLifeBarScript()
    : CScript(PLAYERDAMAGELIFEBARSCRIPT)
    , m_RenderLife(0.f)
    , m_DelayTime(0.f)
{
}

CPlayerDamageLifeBarScript::~CPlayerDamageLifeBarScript()
{
}

void CPlayerDamageLifeBarScript::begin()
{
    if (nullptr == MeshRender())
    {
        GetOwner()->AddComponent(new CMeshRender);
        MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ProgressBarMtrl"));
    }

    MeshRender()->CreateDynamicMaterial();
}

void CPlayerDamageLifeBarScript::tick()
{
    if (m_DelayTime > 0.f)
    {
        m_DelayTime -= DT;
        return;
    }

    CPlayerScript* pPlyaerScript = CGameManagerScript::GetInset()->GetPlayer()->GetScript<CPlayerScript>();
    if (nullptr == pPlyaerScript)
        return;

    float MaxLife = (float)pPlyaerScript->GetMaxLife();
    float CurLife = (float)pPlyaerScript->GetCurLife();

    m_RenderLife = Lerp(m_RenderLife, CurLife, DT * 2.f);

    // Lerp DT 오차값 보정
    if (fabsf(CurLife - m_RenderLife) < 1e-3f)
        m_RenderLife = CurLife;

    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, MaxLife);
    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_1, m_RenderLife);
    MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(1.f, 0.f, 0.f, 1.f));
}
