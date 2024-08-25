#include "pch.h"
#include "CDamageStarEffect.h"

CDamageStarEffect::CDamageStarEffect()
    : CScript(DAMAGESTAREFFECT)
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
    , m_fRadomMizeMaxPower(5.f)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_eTickType, "Tick Type", 1.f);
    AddScriptParam(SCRIPT_PARAM::INT, &m_eSpawnType, "Spawn Type", 1.f);
}

CDamageStarEffect::CDamageStarEffect(const CDamageStarEffect& Origin)
    : CScript(Origin)
    , m_eTickType(DamageStarTickType::Velocity)
    , m_eSpawnType(DamageStarSpawnType::StaticPos)
    , m_eState(DamageStarState::Enter)
    , m_fMaxScale(0.f)
    , m_fScaling(0.f)
    , m_fPower(0.f)
    , m_fThreshHoldValue(0.f)
    , m_fRadomMizeMinScale(10.f)
    , m_fRadomMizeMaxScale(15.f)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_eTickType, "Tick Type", 1.f);
    AddScriptParam(SCRIPT_PARAM::INT, &m_eSpawnType, "Spawn Type", 1.f);
}

CDamageStarEffect::~CDamageStarEffect()
{
}

void CDamageStarEffect::begin()
{
    if (m_eTickType == DamageStarTickType::Velocity)
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
    // ÈûÁÖ±â
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

    if (m_eTickType == DamageStarTickType::Velocity)
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
    if (m_fScaling <= 1.f)
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
    // Track
    CGameObject* pCamObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Editor Camera");

    if (nullptr == pCamObj)
        return;

    // owenr
    CTransform* pTr = GetOwner()->Transform();

    if (nullptr != pTr)
    {
        pTr->SetLocalRotation(pCamObj->Transform()->GetLocalRotation());
    }
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
    return MemoryByte;
}

UINT CDamageStarEffect::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}