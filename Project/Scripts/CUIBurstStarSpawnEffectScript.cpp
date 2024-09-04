#include "pch.h"
#include "CUIBurstStarSpawnEffectScript.h"

CUIBurstStarSpawnEffectScript::CUIBurstStarSpawnEffectScript()
    : CScript(UIBURSTSTARSPAWNEFFECTSCRIPT)
    , m_eState(MonsterBurstStarSpawnState::StarEnter)
{
}

CUIBurstStarSpawnEffectScript::CUIBurstStarSpawnEffectScript(const CUIBurstStarSpawnEffectScript& Origin)
    : CScript(Origin)
    , m_eState(MonsterBurstStarSpawnState::StarEnter)
{
}

CUIBurstStarSpawnEffectScript::~CUIBurstStarSpawnEffectScript()
{
}

void CUIBurstStarSpawnEffectScript::tick()
{
    switch (m_eState)
    {
    case MonsterBurstStarSpawnState::StarEnter: {
        StarEnter();
    }
    break;
    case MonsterBurstStarSpawnState::End: {
        End();
    }
    break;
    default:
        break;
    }
}

void CUIBurstStarSpawnEffectScript::StarEnter()
{
    Vec3 vOwnerPos = GetOwner()->Transform()->GetWorldPos();

    // Common Hit Spawn
    {
        CGameObject* pCommonHitSpawn =
            CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_CommonHit.pref", L"prefab\\Effect_CommonHit.pref")->Instantiate();

        pCommonHitSpawn->Transform()->SetWorldPos(vOwnerPos);

        GamePlayStatic::SpawnGameObject(pCommonHitSpawn, pCommonHitSpawn->GetLayerIdx());
    }

    // Star Spawn
    for (int i = 0; i < 8; i++)
    {
        CGameObject* pStarEffect =
            CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_BurstStar0.pref", L"prefab\\Effect_BurstStar0.pref")->Instantiate();

        pStarEffect->Transform()->SetWorldPos(vOwnerPos);

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());
    }

    for (int i = 0; i < 6; i++)
    {
        CGameObject* pStarEffect =
            CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_BurstStar1.pref", L"prefab\\Effect_BurstStar1.pref")->Instantiate();

        pStarEffect->Transform()->SetWorldPos(vOwnerPos);

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());

        pStarEffect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_BurstStar2.pref", L"prefab\\Effect_BurstStar2.pref")->Instantiate();

        pStarEffect->Transform()->SetWorldPos(vOwnerPos);

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());

        pStarEffect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_BurstStar3.pref", L"prefab\\Effect_BurstStar3.pref")->Instantiate();

        pStarEffect->Transform()->SetWorldPos(vOwnerPos);

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());
    }

    for (int i = 0; i < 6; i++)
    {
        CGameObject* pStarEffect =
            CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_BurstStar4.pref", L"prefab\\Effect_BurstStar4.pref")->Instantiate();

        pStarEffect->Transform()->SetWorldPos(vOwnerPos);

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());

        pStarEffect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_BurstStar5.pref", L"prefab\\Effect_BurstStar5.pref")->Instantiate();

        pStarEffect->Transform()->SetWorldPos(vOwnerPos);

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());

        pStarEffect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_BurstStar6.pref", L"prefab\\Effect_BurstStar6.pref")->Instantiate();

        pStarEffect->Transform()->SetWorldPos(vOwnerPos);

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());

        pStarEffect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_BurstStar7.pref", L"prefab\\Effect_BurstStar7.pref")->Instantiate();

        pStarEffect->Transform()->SetWorldPos(vOwnerPos);

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());
    }

    ChangeState(MonsterBurstStarSpawnState::End);
}

void CUIBurstStarSpawnEffectScript::End()
{
    GamePlayStatic::DestroyGameObject(GetOwner());
}

UINT CUIBurstStarSpawnEffectScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CUIBurstStarSpawnEffectScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}