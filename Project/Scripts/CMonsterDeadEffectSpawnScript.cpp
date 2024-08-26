#include "pch.h"
#include "CMonsterDeadEffectSpawnScript.h"

CMonsterDeadEffectSpawnScript::CMonsterDeadEffectSpawnScript()
    : CScript(MONSTERDEADEFFECTSPAWNSCRIPT)
    , m_eState(MonsterDeadEffectSpawnState::Enter)
    , m_iBodySize(0)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_iBodySize, "Body Size", 1.f);
}

CMonsterDeadEffectSpawnScript::CMonsterDeadEffectSpawnScript(const CMonsterDeadEffectSpawnScript& Origin)
    : CScript(Origin)
    , m_eState(MonsterDeadEffectSpawnState::Enter)
    , m_iBodySize(0)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_iBodySize, "Body Size", 1.f);
}

CMonsterDeadEffectSpawnScript::~CMonsterDeadEffectSpawnScript()
{
}

void CMonsterDeadEffectSpawnScript::tick()
{
    switch (m_eState)
    {
    case MonsterDeadEffectSpawnState::Enter: {
        Enter();
    }
    break;
    case MonsterDeadEffectSpawnState::End: {
        End();
    }
    break;
    default:
        break;
    }
}

void CMonsterDeadEffectSpawnScript::Enter()
{
    // Body Size ºÐ·ù
    wstring strSize = L"";
    if (0 == m_iBodySize)
    {
        strSize = L"(L)";
    }
    else if (1 == m_iBodySize)
    {
        strSize = L"(M)";
    }
    else
    {
        strSize = L"(S)";
    }

    Vec3 vOwnerPos = GetOwner()->Transform()->GetWorldPos();

    // Smoke Spawn
    CGameObject* pSmokeEffect =
        CAssetMgr::GetInst()
            ->Load<CPrefab>(L"prefab\\" + strSize + L"ImpactSmokeSpanwer.pref", L"prefab\\" + strSize + L"ImpactSmokeSpanwer.pref")
            ->Instantiate();

    pSmokeEffect->Transform()->SetWorldPos(vOwnerPos);

    GamePlayStatic::SpawnGameObject(pSmokeEffect, pSmokeEffect->GetLayerIdx());

    // Star Spawn
    for (int i = 0; i < 5; i++)
    {
        CGameObject* pStarEffect =
            CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_StarEffect.pref", L"prefab\\Effect_StarEffect.pref")->Instantiate();

        pStarEffect->Transform()->SetWorldPos(vOwnerPos);

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());
    }

    // Sharp Star Spawn
    for (int i = 0; i < 5; i++)
    {
        CGameObject* pSharpStarEffect =
            CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_SharpStarEffect.pref", L"prefab\\Effect_SharpStarEffect.pref")->Instantiate();

        pSharpStarEffect->Transform()->SetWorldPos(vOwnerPos);

        GamePlayStatic::SpawnGameObject(pSharpStarEffect, pSharpStarEffect->GetLayerIdx());
    }

    ChangeState(MonsterDeadEffectSpawnState::End);
}

void CMonsterDeadEffectSpawnScript::End()
{
    GamePlayStatic::DestroyGameObject(GetOwner());
}

UINT CMonsterDeadEffectSpawnScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_iBodySize, sizeof(int), 1, _File);

    MemoryByte += sizeof(int);

    return MemoryByte;
}

UINT CMonsterDeadEffectSpawnScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_iBodySize, sizeof(int), 1, _File);

    MemoryByte += sizeof(int);

    return MemoryByte;
}