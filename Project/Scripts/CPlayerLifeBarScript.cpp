#include "pch.h"
#include "CPlayerLifeBarScript.h"

#include <Engine\\CLevelMgr.h>
#include <Engine\\CLevel.h>

#include "CPlayerScript.h"

CPlayerLifeBarScript::CPlayerLifeBarScript()
    : CScript(PLAYERLIFEBARSCRIPT)
{
}

CPlayerLifeBarScript::~CPlayerLifeBarScript()
{
}

void CPlayerLifeBarScript::begin()
{
    if (nullptr == MeshRender())
    {
        GetOwner()->AddComponent(new CMeshRender);
        MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ProgressBarMtrl"));
    }

    MeshRender()->CreateDynamicMaterial();
}

void CPlayerLifeBarScript::tick()
{
    CPlayerScript* pPlyaerScript = CGameManagerScript::GetInset()->GetPlayer()->GetScript<CPlayerScript>();
    if (nullptr == pPlyaerScript)
        return;

    float MaxLife = (float)pPlyaerScript->GetMaxLife();
    float CurLife = (float)pPlyaerScript->GetCurLife();

    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, MaxLife);
    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_1, CurLife);
    MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
}
