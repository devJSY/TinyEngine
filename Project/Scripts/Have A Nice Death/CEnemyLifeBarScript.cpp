#include "pch.h"
#include "CEnemyLifeBarScript.h"
#include "CEnemyScript.h"

CEnemyLifeBarScript::CEnemyLifeBarScript()
    : CScript(ENEMYLIFEBARSCRIPT)
    , m_Enemy(nullptr)
{
}

CEnemyLifeBarScript::~CEnemyLifeBarScript()
{
}

void CEnemyLifeBarScript::begin()
{
    if (nullptr == MeshRender())
    {
        GetOwner()->AddComponent(new CMeshRender);
        MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ProgressBarMtrl"));
    }

    MeshRender()->CreateDynamicMaterial();
}

void CEnemyLifeBarScript::tick()
{
    if (nullptr == m_Enemy)
        return;

    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, (float)m_Enemy->GetMaxLife());
    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_1, (float)m_Enemy->GetCurLife());
    MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
}
