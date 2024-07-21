#include "pch.h"
#include "CMonsterUnitScript.h"

CMonsterUnitScript::CMonsterUnitScript(UINT _Type)
    : CUnitScript(_Type)
    , m_pTargetObj(nullptr)
    , m_RaycastDist(100.f)
    , m_HitInfo{}
{
    UnitInfo MonsterInfo = {
        100.f, // HP
        100.f, // MaxHP
        1.f,   // Speed
        3.f,   // Rotation Speed
        1.f,   // JumpPower
    };
    SetInitInfo(MonsterInfo);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.HP, "[Init] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.MAXHP, "[Init] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.Speed, "[Init] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.RotationSpeed, "[Init] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.JumpPower, "[Init] Jump Power");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "[Current] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "[Current] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "[Current] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "[Current] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "[Current] Jump Power");

    AddScriptParam(SCRIPT_PARAM::OBJECT, m_pTargetObj, "Target Object");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RaycastDist, "Raycast Distance");
    AddScriptParam(SCRIPT_PARAM::INT, &m_HitInfo.Type, "DamageType");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HitInfo.Damage, "Damage");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HitInfo.Duration, "DamageDuration");
}

CMonsterUnitScript::CMonsterUnitScript(const CMonsterUnitScript& _Origin)
    : CUnitScript(_Origin)
    , m_pTargetObj(_Origin.m_pTargetObj)
    , m_RaycastDist(_Origin.m_RaycastDist)
    , m_HitInfo(_Origin.m_HitInfo)
{
    UnitInfo MonsterInfo = {
        100.f, // HP
        100.f, // MaxHP
        1.f,   // Speed
        3.f,   // Rotation Speed
        1.f,   // JumpPower
    };
    SetInitInfo(MonsterInfo);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.HP, "[Init] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.MAXHP, "[Init] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.Speed, "[Init] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.RotationSpeed, "[Init] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.JumpPower, "[Init] Jump Power");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "[Current] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "[Current] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "[Current] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "[Current] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "[Current] Jump Power");

    AddScriptParam(SCRIPT_PARAM::OBJECT, m_pTargetObj, "Target Object");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RaycastDist, "Raycast Distance");
    AddScriptParam(SCRIPT_PARAM::INT, &m_HitInfo.Type, "DamageType");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HitInfo.Damage, "Damage");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HitInfo.Duration, "DamageDuration");
}

CMonsterUnitScript::~CMonsterUnitScript()
{
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
    float Angle = Transform()->GetLocalRotation().y;
    Angle += DT * GetCurInfo().RotationSpeed;
    Quat Quaternion = Quat::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), Angle);
    Transform()->SetWorldRotation(Quaternion);
}

void CMonsterUnitScript::RotatingToTarget()
{
    if (nullptr == m_pTargetObj)
        return;

    Vec3 ToTargetDir = Transform()->GetWorldPos() - m_pTargetObj->Transform()->GetWorldPos();
    Transform()->Slerp(ToTargetDir, DT * m_CurInfo.RotationSpeed);
}

bool CMonsterUnitScript::IsGround()
{
    static vector<wstring> vecCollision{L"World Static", L"World Dynamic"};
    RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Transform()->GetWorldPos(), Vec3(0.f, -1.f, 0.f), m_RaycastDist, vecCollision);
    return nullptr != Hit.pCollisionObj;
}

UINT CMonsterUnitScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CUnitScript::SaveToLevelFile(_File);
    fwrite(&m_RaycastDist, 1, sizeof(float), _File);
    fwrite(&m_HitInfo, sizeof(UnitHit), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(UnitHit);

    return MemoryByte;
}

UINT CMonsterUnitScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CUnitScript::LoadFromLevelFile(_File);
    fread(&m_RaycastDist, 1, sizeof(float), _File);
    fread(&m_HitInfo, sizeof(m_HitInfo), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(UnitHit);

    return MemoryByte;
}
