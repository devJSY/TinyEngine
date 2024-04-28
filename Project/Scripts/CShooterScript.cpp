#include "pch.h"
#include "CShooterScript.h"
#include "CProjectileScript.h"

CShooterScript::CShooterScript()
    : CScript(SHOOTERSCRIPT)
{
}

CShooterScript::~CShooterScript()
{
}

void CShooterScript::tick()
{
    if (KEY_TAP(KEY::SPACE))
    {
        Ptr<CPrefab> pref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Test.pref", L"prefab\\Test.pref");
        CGameObject* pObj = pref->Instantiate();
        pObj->Transform()->SetRelativePos(Transform()->GetWorldPos());
        CProjectileScript* script = pObj->GetScript<CProjectileScript>();
        script->SetInitVelocity(Transform()->GetWorldDir(DIR_TYPE::FRONT) * 500.f);
        GamePlayStatic::SpawnGameObject(pObj, 0);
    }
}
