#include "pch.h"
#include "CSmokeSpawner.h"

CSmokeSpawner::CSmokeSpawner()
    : CScript(SMOKESPAWNER)
    , m_SmokePrefabName{}
    , m_bIsOneTime(false)
    , m_bRandom(false)
    , m_MinSpawnCount(1)
    , m_SpawnCoolTime(1.f)
    , m_SpawnMesh(0)
    , m_MaxSpawnCount(5)
    , m_SmokePrefab(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_SmokePrefabName, "Smoke Prefab Name");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bIsOneTime, "Is OneTime?");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRandom, "Is Random?");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_SpawnCoolTime, "SpawnCoolTime");
    AddScriptParam(SCRIPT_PARAM::INT, &m_SpawnMesh, "SpawnMesh (0 : Sphere, 1: Box)");
    AddScriptParam(SCRIPT_PARAM::INT, &m_MinSpawnCount, "MinSpawnCount");
    AddScriptParam(SCRIPT_PARAM::INT, &m_MaxSpawnCount, "MaxSpawnCount");
}

CSmokeSpawner::CSmokeSpawner(const CSmokeSpawner& _Origin)
    : CScript(SMOKESPAWNER)
    , m_SmokePrefabName{_Origin.m_SmokePrefabName}
    , m_bIsOneTime(_Origin.m_bIsOneTime)
    , m_bRandom(_Origin.m_bRandom)
    , m_MinSpawnCount(_Origin.m_MinSpawnCount)
    , m_SpawnCoolTime(_Origin.m_SpawnCoolTime)
    , m_SpawnMesh(_Origin.m_SpawnMesh)
    , m_MaxSpawnCount(_Origin.m_MaxSpawnCount)
    , m_SmokePrefab(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_SmokePrefabName, "Smoke Prefab Name");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bIsOneTime, "Is OneTime?");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRandom, "Is Random?");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_SpawnCoolTime, "SpawnCoolTime");
    AddScriptParam(SCRIPT_PARAM::INT, &m_SpawnMesh, "SpawnMesh (0 : Sphere, 1: Box)");
    AddScriptParam(SCRIPT_PARAM::INT, &m_MinSpawnCount, "MinSpawnCount");
    AddScriptParam(SCRIPT_PARAM::INT, &m_MaxSpawnCount, "MaxSpawnCount");
}

CSmokeSpawner::~CSmokeSpawner()
{
}

void CSmokeSpawner::begin()
{
    if (m_SmokePrefabName == "")
    {
        m_SmokePrefabName = "DefaultSmoke";
    }

    wstring PrefabName = ToWstring(m_SmokePrefabName);

    wstring PrefabPath = L"prefab\\";
    PrefabPath += PrefabName;
    PrefabPath += L".pref";

    m_SmokePrefab = CAssetMgr::GetInst()->Load<CPrefab>(PrefabPath);

    // 프리팹이 없다면 오브젝트를 삭제한다.
    if (m_SmokePrefab == nullptr)
    {
        GetOwner()->SetActive(false);
        GamePlayStatic::DestroyGameObject(GetOwner());
    }

    m_Acc = 0.f;
}

void CSmokeSpawner::tick()
{
    m_Acc += DT;

    // 지속적으로 스폰을 해야한다면
    if (m_bIsOneTime == false)
    {
        // Spawn 시간이 됐다면
        if (m_Acc > m_SpawnCoolTime)
        {
            Spawn();
            m_Acc -= m_SpawnCoolTime;
        }
    }
    // 일시적이라면
    else
    {
        Spawn();
        // 다시 Active 될때 까지 Deactive
        GetOwner()->SetActive(false);
    }
}

void CSmokeSpawner::Spawn()
{
    Vec3 CenterPos = GetOwner()->Transform()->GetWorldPos();

    if (m_bRandom)
    {
        Vec3 Range = GetOwner()->Transform()->GetLocalScale();
        Vec3 Up = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::UP);
        Vec3 Right = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
        Vec3 Front = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);

        switch (m_SpawnMesh)
        {
        case (UINT)SpawnMesh::Sphere: {
            int SpawnCount = (int)SimpleNoise((float)m_MinSpawnCount, (float)m_MaxSpawnCount + 1);
            if (SpawnCount > 0)
            {
                for (int i = 0; i < SpawnCount; ++i)
                {
                    Vec3 TargetPos = CenterPos;

                    Vec3 Dir = SimpleNoise(-1.f, 1.f) * Right + SimpleNoise(-1.f, 1.f) * Up + SimpleNoise(-1.f, 1.f) * Front;
                    Dir.Normalize();

                    TargetPos += Dir * Range.x;

                    CGameObject* Smoke = m_SmokePrefab->Instantiate();
                    Smoke->Transform()->SetWorldPos(TargetPos);

                    GamePlayStatic::SpawnGameObject(Smoke, LAYER_EFFECT);
                }
            }
        }
        break;
        case (UINT)SpawnMesh::Box: {
            int SpawnCount = (int)SimpleNoise((float)m_MinSpawnCount, (float)m_MaxSpawnCount + 1);
            if (SpawnCount > 0)
            {
                for (int i = 0; i < SpawnCount; ++i)
                {
                    Vec3 TargetPos = CenterPos;
                    Vec3 Offset =
                        SimpleNoise(-Range.x, Range.x) * Right + SimpleNoise(-Range.y, Range.y) * Up + SimpleNoise(-Range.z, Range.z) * Front;

                    TargetPos += Offset;

                    CGameObject* Smoke = m_SmokePrefab->Instantiate();
                    Smoke->Transform()->SetWorldPos(TargetPos);

                    GamePlayStatic::SpawnGameObject(Smoke, LAYER_EFFECT);
                }
            }
        }
        break;

        default:
            break;
        }
    }
    else
    {
        // Random이 아니라면 1개를 Pos에 스폰
        CGameObject* Smoke = m_SmokePrefab->Instantiate();
        Smoke->Transform()->SetWorldPos(CenterPos);

        GamePlayStatic::SpawnGameObject(Smoke, LAYER_EFFECT);
    }

}

float CSmokeSpawner::SimpleNoise(float min, float max)
{
    float noise = rand() / float(RAND_MAX);
    return min + (max - min) * noise;
}

UINT CSmokeSpawner::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += SaveWStringToFile(ToWstring(m_SmokePrefabName), _File);
    
    fwrite(&m_bIsOneTime, sizeof(bool), 1, _File);
    fwrite(&m_bRandom, sizeof(bool), 1, _File);

    fwrite(&m_SpawnCoolTime, sizeof(float), 1, _File);
    fwrite(&m_SpawnMesh, sizeof(UINT), 1, _File);

    fwrite(&m_MinSpawnCount, sizeof(int), 1, _File);
    fwrite(&m_MaxSpawnCount, sizeof(int), 1, _File);


    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(UINT);

    MemoryByte += sizeof(int);
    MemoryByte += sizeof(int);

    return MemoryByte;
}

UINT CSmokeSpawner::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring SmokePrefabName;
    MemoryByte += LoadWStringFromFile(SmokePrefabName, _File);
    m_SmokePrefabName = ToString(SmokePrefabName);

    fread(&m_bIsOneTime, sizeof(bool), 1, _File);
    fread(&m_bRandom, sizeof(bool), 1, _File);

    fread(&m_SpawnCoolTime, sizeof(float), 1, _File);
    fread(&m_SpawnMesh, sizeof(UINT), 1, _File);

    fread(&m_MinSpawnCount, sizeof(int), 1, _File);
    fread(&m_MaxSpawnCount, sizeof(int), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(UINT);

    MemoryByte += sizeof(int);
    MemoryByte += sizeof(int);

    return MemoryByte;
}
