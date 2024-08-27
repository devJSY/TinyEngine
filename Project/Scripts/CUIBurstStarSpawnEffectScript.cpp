#include "pch.h"
#include "CUIBurstStarSpawnEffectScript.h"

CUIBurstStarSpawnEffectScript::CUIBurstStarSpawnEffectScript()
    : CScript(UIBURSTSTARSPAWNEFFECTSCRIPT)
    , m_eState(MonsterBurstStarSpawnState::Enter)
{
}

CUIBurstStarSpawnEffectScript::CUIBurstStarSpawnEffectScript(const CUIBurstStarSpawnEffectScript& Origin)
    : CScript(Origin)
    , m_eState(MonsterBurstStarSpawnState::Enter)
{
}

CUIBurstStarSpawnEffectScript::~CUIBurstStarSpawnEffectScript()
{
}

void CUIBurstStarSpawnEffectScript::tick()
{
    switch (m_eState)
    {
    case MonsterBurstStarSpawnState::Enter: {
        Enter();
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

void CUIBurstStarSpawnEffectScript::Enter()
{
    Vec3 vOwnerPos = GetOwner()->Transform()->GetWorldPos();

    // Star Spawn
    for (int i = 0; i < 10; i++)
    {
        CGameObject* pStarEffect =
            CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_BurstStar0.pref", L"prefab\\Effect_BurstStar0.pref")->Instantiate();

        pStarEffect->Transform()->SetWorldPos(vOwnerPos);

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());
    }

    for (int i = 0; i < 5; i++)
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