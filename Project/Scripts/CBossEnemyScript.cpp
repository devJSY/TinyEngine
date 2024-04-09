#include "pch.h"
#include "CBossEnemyScript.h"
#include "CPlayerCameraScript.h"
#include "CCinematicScript.h"
#include "CEnemyLifeBarScript.h"
#include "CEnemyDamageLifeBarScript.h"

CBossEnemyScript::CBossEnemyScript(UINT _ScriptType)
    : CEnemyScript(_ScriptType)
{
}

CBossEnemyScript::~CBossEnemyScript()
{
}

void CBossEnemyScript::SpawnBossUI(BOSS_TYPE _Type)
{
    Ptr<CPrefab> pHUD = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\BossUI.pref", L"prefab\\BossUI.pref");
    if (nullptr != pHUD)
    {
        CGameObject* pHUDObj = pHUD->Instantiate();

        const vector<CGameObject*>& vecChild = pHUDObj->GetChildObject();

        for (UINT i = 0; i < vecChild.size(); i++)
        {
            CEnemyLifeBarScript* pLifeBarScript = vecChild[i]->GetScript<CEnemyLifeBarScript>();
            CEnemyDamageLifeBarScript* pDamageLifeBarScript = vecChild[i]->GetScript<CEnemyDamageLifeBarScript>();

            if (nullptr != pLifeBarScript)
                pLifeBarScript->SetEnemy(this);

            if (nullptr != pDamageLifeBarScript)
                pDamageLifeBarScript->SetEnemy(this);
        }

        Ptr<CMaterial> pBossIcon = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\HUD_Boss_IconMtrl.mtrl");
        if (nullptr != pBossIcon)
        {
            switch (_Type)
            {
            case BOSS_TYPE::LIFE: {
                pBossIcon->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\UI\\HUD\\HUD_Elements_Boss_Icon_Natalie.png",
                                                                                   L"Texture\\UI\\HUD\\HUD_Elements_Boss_Icon_Natalie.png"));
            }
            break;
            case BOSS_TYPE::EVILPUMPKIN: {
                pBossIcon->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\UI\\HUD\\HUD_Elements_Miniboss_Icon_EvilPumpkin.png",
                                                                                   L"Texture\\UI\\HUD\\HUD_Elements_Miniboss_Icon_EvilPumpkin.png"));
            }
            break;
            }
        }

        int UIIdx = 0;
        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

        for (int i = 0; i < LAYER_MAX; i++)
        {
            if (L"UI" == pCurLevel->GetLayer(i)->GetName())
            {
                UIIdx = i;
                break;
            }
        }

        GamePlayStatic::SpawnGameObject(pHUDObj, UIIdx);
    }
}

void CBossEnemyScript::DestroyBossUI()
{
    CGameObject* pBossUI = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"BossUI");
    if (nullptr != pBossUI)
    {
        GamePlayStatic::DestroyGameObject(pBossUI);
    }
}

void CBossEnemyScript::DamageLifeBarUpdate()
{
    CGameObject* pDamageLifeBar = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectOfType<CEnemyDamageLifeBarScript>();
    if (nullptr != pDamageLifeBar)
    {
        pDamageLifeBar->GetScript<CEnemyDamageLifeBarScript>()->TakeHit(1.f);
    }
}

void CBossEnemyScript::EndBossBattle()
{
    // Player Camera Lock Off
    CGameObject* pPlayerCamObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"PlayerCamera");
    if (nullptr != pPlayerCamObj)
    {
        CPlayerCameraScript* pScript = pPlayerCamObj->GetScript<CPlayerCameraScript>();
        if (nullptr != pScript)
        {
            pScript->Lock(false);
        }
    }

    // Wall Collider Disable
    CGameObject* pCinematicObject = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"CinematicObject");
    if (nullptr != pCinematicObject)
    {
        CCinematicScript* pCinematicScript = pCinematicObject->GetScript<CCinematicScript>();
        if (nullptr != pCinematicScript)
        {
            pCinematicScript->SetEnableWallCollider(false);
        }
    }
}

void CBossEnemyScript::SaveToLevelFile(FILE* _File)
{
    CEnemyScript::SaveToLevelFile(_File);
}

void CBossEnemyScript::LoadFromLevelFile(FILE* _File)
{
    CEnemyScript::LoadFromLevelFile(_File);
}
