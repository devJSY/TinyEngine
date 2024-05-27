#include "pch.h"
#include "CPlayerManaBarScript.h"

#include <Engine\\CLevelMgr.h>
#include <Engine\\CLevel.h>

#include "CPlayerScript.h"

CPlayerManaBarScript::CPlayerManaBarScript()
    : CScript(PLAYERMANABARSCRIPT)
{
}

CPlayerManaBarScript::~CPlayerManaBarScript()
{
}

void CPlayerManaBarScript::begin()
{
    if (nullptr == MeshRender())
    {
        GetOwner()->AddComponent(new CMeshRender);
        MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ProgressBarMtrl"));
    }

    MeshRender()->CreateDynamicMaterial();
}

void CPlayerManaBarScript::tick()
{
    CPlayerScript* pPlyaerScript = CGameManagerScript::GetInset()->GetPlayer()->GetScript<CPlayerScript>();
    if (nullptr == pPlyaerScript)
        return;

    float MaxMana = (float)pPlyaerScript->GetMaxMana();
    float CurMana = (float)pPlyaerScript->GetCurMana();

    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, MaxMana);
    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_1, CurMana);
    MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(0.f, 1.f, 0.f, 1.f));
}
