#include "pch.h"
#include "CDamageStarEffect.h"

#include "CPlayerMgr.h"
#include "CCameraController.h"

CDamageStarEffect::CDamageStarEffect()
    : CScript(DAMAGESTAREFFECT)
    , m_bUseGravity(false)
    , m_eTickType(DamageStarTickType::Velocity)
    , m_eSpawnType(DamageStarSpawnType::StaticPos)
    , m_eState(DamageStarState::Enter)
    , m_fMaxScale(0.f)
    , m_fScaling(0.f)
    , m_fPower(0.f)
    , m_fThreshHoldValue(0.f)
    , m_fRadomMizeMinScale(10.f)
    , m_fRadomMizeMaxScale(15.f)
    , m_fRandomMizeMinPower(1.f)
    , m_fRandomMizeMaxPower(5.f)
{
    // Common Value
    AddScriptParam(SCRIPT_PARAM::INT, &m_eTickType, "Tick Type", 1.f);
    AddScriptParam(SCRIPT_PARAM::INT, &m_eSpawnType, "Spawn Type", 1.f);
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bUseGravity, "Use Gravity", 1.f);

    // Static Value
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxScale, "Max Scaling", 1.f);
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Static Direction", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fPower, "Static Power", 1.f);

    // Radnom Value
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRadomMizeMinScale, "Random Min Scale", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRadomMizeMaxScale, "Random Max Scale", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRandomMizeMinPower, "Random Min Power", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRandomMizeMaxPower, "Random Max Power", 1.f);
}

CDamageStarEffect::CDamageStarEffect(const CDamageStarEffect& Origin)
    : CScript(Origin)
    , m_bUseGravity(Origin.m_bUseGravity)
    , m_eTickType(Origin.m_eTickType)
    , m_eSpawnType(Origin.m_eSpawnType)
    , m_eState(DamageStarState::Enter)
    , m_fMaxScale(Origin.m_fMaxScale)
    , m_fScaling(Origin.m_fScaling)
    , m_fPower(Origin.m_fPower)
    , m_fThreshHoldValue(0.f)
    , m_fRadomMizeMinScale(Origin.m_fRadomMizeMinScale)
    , m_fRadomMizeMaxScale(Origin.m_fRadomMizeMaxScale)
    , m_fRandomMizeMinPower(Origin.m_fRandomMizeMinPower)
    , m_fRandomMizeMaxPower(Origin.m_fRandomMizeMaxPower)
{
    // Common Value
    AddScriptParam(SCRIPT_PARAM::INT, &m_eTickType, "Tick Type", 1.f);
    AddScriptParam(SCRIPT_PARAM::INT, &m_eSpawnType, "Spawn Type", 1.f);
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bUseGravity, "Use Gravity", 1.f);

    // Static Value
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxScale, "Max Scaling", 1.f);
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Static Direction", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fPower, "Static Power", 1.f);

    // Radnom Value
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRadomMizeMinScale, "Random Min Scale", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRadomMizeMaxScale, "Random Max Scale", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRandomMizeMinPower, "Random Min Power", 1.f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRandomMizeMaxPower, "Random Max Power", 1.f);
}

CDamageStarEffect::~CDamageStarEffect()
{
}

void CDamageStarEffect::begin()
{
    if (m_bUseGravity)
    {
        Rigidbody()->SetUseGravity(true);
    }
    else
    {
        Rigidbody()->SetUseGravity(false);
    }
}

void CDamageStarEffect::tick()
{
    switch (m_eState)
    {
    case DamageStarState::Enter: {
        Enter();
    }
    break;
    case DamageStarState::UpState: {
        UpState();
    }
    break;
    case DamageStarState::DownState: {
        DownState();
    }
    break;
    case DamageStarState::Exit: {
        Exit();
    }
    break;
    case DamageStarState::End:
        break;
    default:
        break;
    }

    TrackCamera();
}

void CDamageStarEffect::Enter()
{
    if (m_eTickType == DamageStarTickType::Velocity)
    {
        Vec3 vRandomDir = RadomizeDir();
        float fPower = RandomizePow();
        m_fMaxScale = RandomizeScale();

        if (Rigidbody())
        {
            Rigidbody()->AddForce(vRandomDir * fPower, ForceMode::Impulse);
        }
    }

    if (m_eTickType == DamageStarTickType::Direction)
    {
        if (Rigidbody())
        {
            Rigidbody()->AddForce(m_vDir * m_fPower, ForceMode::Acceleration);
        }
    }

    ChangeState(DamageStarState::UpState);
}

void CDamageStarEffect::UpState()
{
    Scaling(true);
    if (m_fScaling >= m_fMaxScale - 1.f)
    {
        ChangeState(DamageStarState::DownState);
    }
}

void CDamageStarEffect::DownState()
{
    Scaling(false);
    if (m_fScaling <= 1.1f)
    {
        ChangeState(DamageStarState::Exit);
    }
}

void CDamageStarEffect::Exit()
{
    GamePlayStatic::DestroyGameObject(GetOwner());
}

void CDamageStarEffect::TrackCamera()
{
    Vec3 LookDir = (CAMERACTRL->GetOwner()->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
    Transform()->SetDirection(LookDir);
}

void CDamageStarEffect::Scaling(bool _bFlag)
{
    if (_bFlag)
    {
        m_fScaling = Lerp(m_fScaling, m_fMaxScale, DT * 4.f);
    }
    else
    {
        m_fScaling = Lerp(m_fScaling, 1.f, DT * 2.f);
    }

    Transform()->SetWorldScale(Vec3(1.f * m_fScaling, 1.f * m_fScaling, 1.f));
}

Vec3 CDamageStarEffect::RadomizeDir()
{
    int Random = GetRandomInt(0, 3);
    Vec3 vDir = Vec3(0.f, 0.f, 0.f);

    if (0 == Random)
    {
        vDir = Vec3(0.5f, 1.f, 0.f);
    }
    else if (1 == Random)
    {
        vDir = Vec3(0.5f, 1.f, 0.f);
    }
    else if (2 == Random)
    {
        vDir = Vec3(-1.f, 1.f, 0.f);
    }
    else
    {
        vDir = Vec3(-0.5f, 1.f, 0.f);
    }

    return vDir;
}

float CDamageStarEffect::RandomizeScale()
{
    return GetRandomfloat(m_fRadomMizeMinScale, m_fRadomMizeMaxScale);
}

float CDamageStarEffect::RandomizePow()
{
    return GetRandomfloat(1.f, 5.f);
}

UINT CDamageStarEffect::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bUseGravity, sizeof(bool), 1, _File);
    fwrite(&m_eTickType, sizeof(UINT), 1, _File);
    fwrite(&m_eSpawnType, sizeof(UINT), 1, _File);
    fwrite(&m_fMaxScale, sizeof(float), 1, _File);
    fwrite(&m_fPower, sizeof(float), 1, _File);
    fwrite(&m_fRadomMizeMinScale, sizeof(float), 1, _File);
    fwrite(&m_fRadomMizeMaxScale, sizeof(float), 1, _File);
    fwrite(&m_fRandomMizeMinPower, sizeof(float), 1, _File);
    fwrite(&m_fRandomMizeMaxPower, sizeof(float), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(UINT);
    MemoryByte += sizeof(UINT);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CDamageStarEffect::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bUseGravity, sizeof(bool), 1, _File);
    fread(&m_eTickType, sizeof(UINT), 1, _File);
    fread(&m_eSpawnType, sizeof(UINT), 1, _File);
    fread(&m_fMaxScale, sizeof(float), 1, _File);
    fread(&m_fPower, sizeof(float), 1, _File);
    fread(&m_fRadomMizeMinScale, sizeof(float), 1, _File);
    fread(&m_fRadomMizeMaxScale, sizeof(float), 1, _File);
    fread(&m_fRandomMizeMinPower, sizeof(float), 1, _File);
    fread(&m_fRandomMizeMaxPower, sizeof(float), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(UINT);
    MemoryByte += sizeof(UINT);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}