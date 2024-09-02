#include "pch.h"
#include "CMonsterUnitScript.h"

CMonsterUnitScript::CMonsterUnitScript(UINT _Type)
    : CUnitScript(_Type)
    , m_pTargetObj(nullptr)
    , m_RaycastDist(100.f)
    , m_bEatable(true)
    , m_bSparkleOnOff(false)
    , m_bSparkle(false)
    , m_fAccTime(0.f)
    , m_fTermTime(0.f)
    , m_fResistTime(0.f)
{
    UnitInfo MonsterInfo = {
        100.f, // HP
        100.f, // MaxHP
        1.f,   // Speed
        3.f,   // Rotation Speed
        1.f,   // JumpPower
        10.f,  // ATK
    };
    SetInitInfo(MonsterInfo);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.HP, "[Init] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.MAXHP, "[Init] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.Speed, "[Init] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.RotationSpeed, "[Init] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.JumpPower, "[Init] Jump Power");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.ATK, "[Init] Attack Points");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "[Current] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "[Current] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "[Current] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "[Current] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "[Current] Jump Power");

    AddScriptParam(SCRIPT_PARAM::OBJECT, m_pTargetObj, "Target Object");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RaycastDist, "Raycast Distance");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bEatable, "Is Eatable");
}

CMonsterUnitScript::CMonsterUnitScript(const CMonsterUnitScript& _Origin)
    : CUnitScript(_Origin)
    , m_pTargetObj(_Origin.m_pTargetObj)
    , m_RaycastDist(_Origin.m_RaycastDist)
    , m_bEatable(_Origin.m_bEatable)
    , m_bSparkleOnOff(false)
    , m_bSparkle(false)
    , m_fAccTime(0.f)
    , m_fTermTime(0.f)
    , m_fResistTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.HP, "[Init] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.MAXHP, "[Init] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.Speed, "[Init] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.RotationSpeed, "[Init] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.JumpPower, "[Init] Jump Power");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.ATK, "[Init] Attack Points");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "[Current] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "[Current] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "[Current] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "[Current] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "[Current] Jump Power");

    AddScriptParam(SCRIPT_PARAM::OBJECT, m_pTargetObj, "Target Object");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RaycastDist, "Raycast Distance");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bEatable, "Is Eatable");
}

CMonsterUnitScript::~CMonsterUnitScript()
{
}

void CMonsterUnitScript::SpawnDeadEffect(int _iSize)
{
    CGameObject* pSpawnEffect = nullptr;
    if (0 == _iSize)
    {
        pSpawnEffect = CAssetMgr::GetInst()
                           ->Load<CPrefab>(L"prefab\\(L)Effect_MonsterDieEffect.pref", L"prefab\\(L)Effect_MonsterDieEffect.pref")
                           ->Instantiate();
    }
    else if (1 == _iSize)
    {
        pSpawnEffect = CAssetMgr::GetInst()
                           ->Load<CPrefab>(L"prefab\\(M)Effect_MonsterDieEffect.pref", L"prefab\\(M)Effect_MonsterDieEffect.pref")
                           ->Instantiate();
    }
    else
    {
        pSpawnEffect = CAssetMgr::GetInst()
                           ->Load<CPrefab>(L"prefab\\(M)Effect_MonsterDieEffect.pref", L"prefab\\(M)Effect_MonsterDieEffect.pref")
                           ->Instantiate();
    }

    pSpawnEffect->Transform()->SetWorldPos(GetOwner()->Transform()->GetWorldPos());
    GamePlayStatic::SpawnGameObject(pSpawnEffect, pSpawnEffect->GetLayerIdx());
}

void CMonsterUnitScript::begin()
{
    CUnitScript::begin();
    SparkleReset();
}

void CMonsterUnitScript::tick()
{
    CUnitScript::tick();
    SparkleEffect();
}

void CMonsterUnitScript::RigidbodyMove(CGameObject* _pTargetObj)
{
    // 전방 방향으로 이동
    if (nullptr == _pTargetObj)
    {
        Vec3 Dir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Rigidbody()->SetVelocity(Dir * m_CurInfo.Speed);
    }
    // 타겟 방향으로 이동
    else
    {
        Vec3 ToTargetDir = m_pTargetObj->Transform()->GetWorldPos() - Transform()->GetWorldPos();
        ToTargetDir.y = 0.f; // Y축 고정
        ToTargetDir.Normalize();
        Rigidbody()->SetVelocity(ToTargetDir * m_CurInfo.Speed);
    }
}

void CMonsterUnitScript::Rotating()
{
    Quat Quaternion = Quat::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), DT * GetCurInfo().RotationSpeed);
    Transform()->SetWorldRotation(Transform()->GetWorldQuaternion() * Quaternion);
}

void CMonsterUnitScript::RotatingToTarget()
{
    if (nullptr == m_pTargetObj)
        return;

    Vec3 ToTargetDir = m_pTargetObj->Transform()->GetWorldPos() - Transform()->GetWorldPos();
    ToTargetDir.y = 0.f; // Y축 고정
    ToTargetDir.Normalize();
    Transform()->Slerp(ToTargetDir, DT * m_CurInfo.RotationSpeed);
}

bool CMonsterUnitScript::IsGround()
{
    static vector<wstring> vecCollision{L"World Static", L"World Dynamic"};
    RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Transform()->GetWorldPos(), Vec3(0.f, -1.f, 0.f), m_RaycastDist, vecCollision);
    return nullptr != Hit.pCollisionObj;
}

void CMonsterUnitScript::SparkleEffect()
{
    if (m_bSparkle)
    {
        m_fAccTime += DT;
        m_fTermTime += DT;

        if (m_fTermTime >= 0.08f)
        {
            CMeshRender* pMeshRender = MeshRender();
            for (UINT i = 0; i < pMeshRender->GetMtrlCount(); ++i)
            {
                if (m_bSparkleOnOff)
                {
                    pMeshRender->GetDynamicMaterial(i)->SetEmission(Vec4(60.f / 255.f, 60.f / 255.f, 60.f / 255.f, 1.f));
                }
                else
                {
                    pMeshRender->GetDynamicMaterial(i)->SetEmission(Vec4(0.f, 0.f, 0.f, 0.f));
                }
            }
            m_bSparkleOnOff = !m_bSparkleOnOff;
            m_fTermTime = 0.f;
        }
    }

    if (m_fAccTime >= 2.5f)
    {
        SparkleReset();
    }
}

void CMonsterUnitScript::SparkleReset()
{
    CMeshRender* pMeshRender = MeshRender();
    m_bSparkle = false;
    m_fAccTime = 0.f;
    m_fTermTime = 0.f;

    for (UINT i = 0; i < pMeshRender->GetMtrlCount(); ++i)
    {
        pMeshRender->GetMaterial(i)->SetEmission(Vec4(0.f, 0.f, 0.f, 0.f));
        pMeshRender->GetSharedMaterial(i);
    }
}

UINT CMonsterUnitScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CUnitScript::SaveToLevelFile(_File);
    fwrite(&m_RaycastDist, 1, sizeof(float), _File);
    fwrite(&m_bEatable, sizeof(bool), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CMonsterUnitScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CUnitScript::LoadFromLevelFile(_File);
    fread(&m_RaycastDist, 1, sizeof(float), _File);
    fread(&m_bEatable, sizeof(bool), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}
