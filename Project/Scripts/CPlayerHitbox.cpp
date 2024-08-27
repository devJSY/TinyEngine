#include "pch.h"
#include "CPlayerHitbox.h"
#include "CPlayerMgr.h"
#include "CKirbyUnitScript.h"

CPlayerHitbox::CPlayerHitbox()
    : CScript(PLAYERHITBOX)
    , m_Instigator(nullptr)
    , m_Collider(nullptr)
    , m_bSummon(false)
    , m_bCallReward(true)
    , m_Damage(5.f)
    , m_DamageTypeIdx(0)
    , m_AccTime(0.f)
    , m_RepeatTime(2.f)
    , m_bRepeatDamage(true)
    , m_bRepeatEnter(false)
    , m_bRepeat(false)
    , m_CoolTime(0.3f)
    , m_PrevAttackTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Damage, "Damage");
    AddScriptParam(SCRIPT_PARAM::INT, &m_DamageTypeIdx, "Damage Type");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RepeatTime, "Repeat Time");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRepeatDamage, "Damage Repeat (stay trigger)");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCallReward, "Call Reward");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bSummon, "Summon");
}

CPlayerHitbox::CPlayerHitbox(const CPlayerHitbox& _Origin)
    : CScript(_Origin)
    , m_Instigator(nullptr)
    , m_Collider(nullptr)
    , m_bSummon(_Origin.m_bSummon)
    , m_bCallReward(_Origin.m_bCallReward)
    , m_Damage(_Origin.m_Damage)
    , m_DamageTypeIdx(_Origin.m_DamageTypeIdx)
    , m_AccTime(0.f)
    , m_RepeatTime(_Origin.m_RepeatTime)
    , m_bRepeatDamage(_Origin.m_bRepeatDamage)
    , m_bRepeatEnter(false)
    , m_bRepeat(false)
    , m_CoolTime(_Origin.m_CoolTime)
    , m_PrevAttackTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Damage, "Damage");
    AddScriptParam(SCRIPT_PARAM::INT, &m_DamageTypeIdx, "Damage Type");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RepeatTime, "Repeat Time");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRepeatDamage, "Damage Repeat (stay trigger)");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCallReward, "Call Reward");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bSummon, "Summon");
}

CPlayerHitbox::~CPlayerHitbox()
{
}

void CPlayerHitbox::begin()
{
    m_Instigator = PLAYER;
    m_Collider = GetOwner()->GetComponent<CCollider>();
}

void CPlayerHitbox::tick()
{
    if (m_bRepeat)
    {
        m_bRepeat = false;
        m_bRepeatEnter = false;
        m_Collider->SetEnabled(true);
    }

    // case : repeat
    if (!m_Instigator || !m_bRepeatDamage || !m_bRepeatEnter)
        return;

    m_AccTime += DT;

    if (m_AccTime > m_RepeatTime)
    {
        m_AccTime = 0.f;
        m_bRepeat = true;
        m_Collider->SetEnabled(false);
    }
}

void CPlayerHitbox::OnTriggerEnter(CCollider* _OtherCollider)
{

    UINT Layer = _OtherCollider->GetOwner()->GetLayerIdx();
    wstring Name = _OtherCollider->GetOwner()->GetName();
    CGameObject* pMonster = nullptr;

    if (Layer == LAYER_MONSTER)
    {
        pMonster = _OtherCollider->GetOwner();
    }
    else if (Layer == LAYER_MONSTER_TRIGGER)
    {
        if (Name == L"Body Collider")
        {
            pMonster = _OtherCollider->GetOwner()->GetParent();
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }

    m_bRepeatEnter = true;
    m_AccTime = 0.f;
    AddDamage(pMonster);

    CTimeMgr::GetInst()->SetTimeScale(0.1f, 0.f);
}

void CPlayerHitbox::AddDamage(CGameObject* _Monster)
{
    CUnitScript* pUnit = _Monster->GetScript<CUnitScript>();

    if (!m_Instigator || !pUnit)
        return;

    // create HitInfo
    Vec3 HitDir = _Monster->Transform()->GetWorldPos();

    if (!m_bSummon)
    {
        HitDir -= m_Instigator->Transform()->GetWorldPos();
    }
    else
    {
        HitDir -= Transform()->GetWorldPos();
    }
    HitDir.Normalize();

    UnitHit HitInfo = {(DAMAGE_TYPE)m_DamageTypeIdx, HitDir, m_Damage, 0.f, 0.f};

    // Damage & Reward
    pUnit->GetDamage(HitInfo);

    if (m_bCallReward)
    {
        PLAYERUNIT->AttackReward();
    }
}

UINT CPlayerHitbox::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_Damage, 1, sizeof(float), _File);
    fwrite(&m_DamageTypeIdx, 1, sizeof(int), _File);
    fwrite(&m_bRepeatDamage, 1, sizeof(bool), _File);
    fwrite(&m_RepeatTime, 1, sizeof(float), _File);
    fwrite(&m_bSummon, 1, sizeof(bool), _File);
    fwrite(&m_bCallReward, 1, sizeof(bool), _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(int);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CPlayerHitbox::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_Damage, 1, sizeof(float), _File);
    fread(&m_DamageTypeIdx, 1, sizeof(int), _File);
    fread(&m_bRepeatDamage, 1, sizeof(bool), _File);
    fread(&m_RepeatTime, 1, sizeof(float), _File);
    fread(&m_bSummon, 1, sizeof(bool), _File);
    fread(&m_bCallReward, 1, sizeof(bool), _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(int);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}