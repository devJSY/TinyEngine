#include "pch.h"
#include "CBossHitbox.h"
#include "CPlayerMgr.h"
#include "CBossMgr.h"
#include "CKirbyUnitScript.h"

CBossHitbox::CBossHitbox()
    : CScript(BOSSHITBOX)
    , m_Instigator(nullptr)
    , m_Target(nullptr)
    , m_Collider(nullptr)
    , m_bSummon(false)
    , m_bCallReward(true)
    , m_RandMin(5.f)
    , m_RandMax(5.f)
    , m_DamageTypeIdx(0)
    , m_AccTime(0.f)
    , m_RepeatTime(2.f)
    , m_bRepeatDamage(true)
    , m_bRepeat(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RandMin, "Damage (min)");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RandMax, "Damage (max)");
    AddScriptParam(SCRIPT_PARAM::INT, &m_DamageTypeIdx, "Damage Type");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RepeatTime, "Repeat Time");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRepeatDamage, "Damage Repeat (stay trigger)");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCallReward, "Call Reward");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bSummon, "Summon");
}

CBossHitbox::CBossHitbox(const CBossHitbox& _Origin)
    : CScript(_Origin)
    , m_Instigator(nullptr)
    , m_Target(nullptr)
    , m_Collider(nullptr)
    , m_bSummon(_Origin.m_bSummon)
    , m_bCallReward(_Origin.m_bCallReward)
    , m_RandMin(_Origin.m_RandMin)
    , m_RandMax(_Origin.m_RandMax)
    , m_DamageTypeIdx(_Origin.m_DamageTypeIdx)
    , m_AccTime(0.f)
    , m_RepeatTime(_Origin.m_RepeatTime)
    , m_bRepeatDamage(_Origin.m_bRepeatDamage)
    , m_bRepeat(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RandMin, "Damage (min)");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RandMax, "Damage (max)");
    AddScriptParam(SCRIPT_PARAM::INT, &m_DamageTypeIdx, "Damage Type");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RepeatTime, "Repeat Time");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRepeatDamage, "Damage Repeat (stay trigger)");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCallReward, "Call Reward");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bSummon, "Summon");
}

CBossHitbox::~CBossHitbox()
{
}

void CBossHitbox::begin()
{
    m_Target = PLAYER;
    m_Instigator = BOSS;
    m_Collider = GetOwner()->GetComponent<CCollider>();
}

void CBossHitbox::tick()
{
    if (!m_Instigator || !m_Target || !m_bRepeatDamage)
        return;

    m_AccTime += DT;

    if (m_AccTime > m_RepeatTime)
    {
        m_AccTime = 0.f;
        m_bRepeat = true;
    }
}

// 예상문제 : 제일 마지막에 충돌된 몬스터 기준으로 데미지 반복됨
// 시나리오 : 
// - 으르르피랑 먼저 부딪힘
// - 4초 지나는 시점에 황소랑 충돌
// - 데미지 반복해주려고 체크하던 m_AccTime 초기화됨
// - 마지막으로 충돌한 몬스터(황소) 이후 5초 지나서야
// - 충돌했던 모든 몬스터(으르르피, 황소) 모두 데미지 다시 받을 수 있음
// 종합의견 : 데미지 반복 시간이 짧으니 (0.1초~) 문제 뚜렷이 드러나기 전까진 그냥 쓰자 
void CBossHitbox::OnTriggerEnter(CCollider* _OtherCollider)
{
    UINT Layer = _OtherCollider->GetOwner()->GetLayerIdx();
    wstring Name = _OtherCollider->GetOwner()->GetName();
    if (!(Layer == LAYER_PLAYER && Name == L"Main Player") && !(Layer == LAYER_PLAYER_TRIGGER && Name == L"Body Collider"))
        return;

    m_AccTime = 0.f;
    AddDamage();
}

void CBossHitbox::OnTriggerStay(CCollider* _OtherCollider)
{
    UINT Layer = _OtherCollider->GetOwner()->GetLayerIdx();
    wstring Name = _OtherCollider->GetOwner()->GetName();
    if (!(Layer == LAYER_PLAYER && Name == L"Main Player") && !(Layer == LAYER_PLAYER_TRIGGER && Name == L"Body Collider"))
        return;

    if (m_bRepeat)
    {
        m_bRepeat = false;
        AddDamage();
    }
}

float CBossHitbox::GetRandDamage()
{
    float Damage = 5.f;
    Damage = GetRandomfloat(m_RandMin, m_RandMax);

    return Damage;
}

void CBossHitbox::AddDamage()
{
    if (!m_Instigator || !m_Target)
        return;

    // create HitInfo
    Vec3 HitDir = m_Target->Transform()->GetWorldPos();
    if (!m_bSummon)
    {
        HitDir -= m_Instigator->Transform()->GetWorldPos();
    }
    else
    {
        HitDir -= Transform()->GetWorldPos();
    }

    HitDir.Normalize();
    UnitHit HitInfo = {(DAMAGE_TYPE)m_DamageTypeIdx, HitDir, GetRandDamage(), 0.f, 0.f};

    // Damage & Reward
    PLAYERUNIT->GetDamage(HitInfo);
    if (m_bCallReward)
    {
        BOSSUNIT->AttackReward();
    }
}

UINT CBossHitbox::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_RandMin, 1, sizeof(float), _File);
    fwrite(&m_RandMax, 1, sizeof(float), _File);
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

UINT CBossHitbox::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_RandMin, 1, sizeof(float), _File);
    fread(&m_RandMax, 1, sizeof(float), _File);
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
