#include "pch.h"
#include "CEnemyDamageLifeBarScript.h"
#include "CEnemyScript.h"

CEnemyDamageLifeBarScript::CEnemyDamageLifeBarScript()
    : CScript(ENEMYDAMAGELIFEBARSCRIPT)
    , m_Enemy(nullptr)
    , m_RenderLife(0.f)
    , m_DelayTime(0.f)
{
}

CEnemyDamageLifeBarScript::~CEnemyDamageLifeBarScript()
{
}

void CEnemyDamageLifeBarScript::begin()
{
    if (nullptr == MeshRender())
    {
        GetOwner()->AddComponent(new CMeshRender);
        MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ProgressBarMtrl"));
    }

    MeshRender()->CreateDynamicMaterial();
}

void CEnemyDamageLifeBarScript::tick()
{
    if (nullptr == m_Enemy)
        return;

    if (m_DelayTime > 0.f)
    {
        m_DelayTime -= DT;
        return;
    }

    float MaxLife = (float)m_Enemy->GetMaxLife();
    float CurLife = (float)m_Enemy->GetCurLife();

    m_RenderLife = Lerp(m_RenderLife, CurLife, DT * 2.f);

    // Lerp DT 오차값 보정
    if (fabsf(CurLife - m_RenderLife) < 1e-3f)
        m_RenderLife = CurLife;

    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, MaxLife);
    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_1, m_RenderLife);
    MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(0.4f, 0.16f, 0.15f, 1.f));
}
