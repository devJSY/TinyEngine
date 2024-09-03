#include "pch.h"
#include "CUIDashEffectScript.h"

CUIDashEffectScript::CUIDashEffectScript()
    : CScript(UIDASHEFFECTSCRIPT)
    , m_eState(DashEffectState::Start)
    , m_fDashTime(0.f)
    , m_fAccTime(0.f)
    , m_fRatio(0.f)
    , m_fRatioSpeed(0.f)
    , m_fTextureValue(0.f)
    , m_fTextureValueSpeed(0.f)
    , m_fMaxThreshHold(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRatioSpeed, "Dash Ratio Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTextureValueSpeed, "TextureValue Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxThreshHold, "Max ThreshHold");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDashTime, "Dash Time");
}

CUIDashEffectScript::CUIDashEffectScript(const CUIDashEffectScript& Origin)
    : CScript(Origin)
    , m_eState(DashEffectState::Start)
    , m_fDashTime(Origin.m_fDashTime)
    , m_fAccTime(0.f)
    , m_fRatio(0.f)
    , m_fRatioSpeed(Origin.m_fRatioSpeed)
    , m_fTextureValue(0.f)
    , m_fTextureValueSpeed(Origin.m_fTextureValueSpeed)
    , m_fMaxThreshHold(Origin.m_fMaxThreshHold)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRatioSpeed, "Dash Ratio Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTextureValueSpeed, "TextureValue Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxThreshHold, "Max ThreshHold");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDashTime, "Dash Time");
}

CUIDashEffectScript::~CUIDashEffectScript()
{
}

void CUIDashEffectScript::SetBackGroundAlpha(const float _fValue)
{
    if (MeshRender())
    {
        MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_2, _fValue);
    }
}

void CUIDashEffectScript::begin()
{
    ChangeState(DashEffectState::Start);
    m_fRatioSpeed = 0.3;
    MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_2, 0.5f);
    MeshRender()->SetCastShadow(false);
}

void CUIDashEffectScript::tick()
{
    switch (m_eState)
    {
    case DashEffectState::Start: {
        Start();
    }
    break;
    case DashEffectState::Progress: {
        Progress();
    }
    break;
    case DashEffectState::Stop: {
        Stop();
    }
    break;
    case DashEffectState::End:
        break;
    default:
        break;
    }
}

void CUIDashEffectScript::ChangeState(DashEffectState _eState)
{
    Exit();
    m_eState = _eState;
    Enter();
}

void CUIDashEffectScript::Enter()
{
    switch (m_eState)
    {
    case DashEffectState::Start: {
        CMeshRender* pRender = MeshRender();
        if (nullptr != pRender)
        {
            pRender->GetMaterial(0)->SetScalarParam(INT_1, 0);
        }
    }
    break;
    case DashEffectState::Progress: {
        CMeshRender* pRender = MeshRender();
        if (nullptr != pRender)
        {
            pRender->GetMaterial(0)->SetScalarParam(INT_1, 1);
        }
        m_fAccTime = 0.f;
    }
    break;
    case DashEffectState::Stop:
        break;
    case DashEffectState::End:
        break;
    default:
        break;
    }
}

void CUIDashEffectScript::Exit()
{
    switch (m_eState)
    {
    case DashEffectState::Start:
        break;
    case DashEffectState::Progress:
        break;
    case DashEffectState::Stop:
        break;
    case DashEffectState::End:
        break;
    default:
        break;
    }
}

void CUIDashEffectScript::Start()
{
    CMeshRender* pRender = MeshRender();
    m_fAccTime += DT;

    float fRatio = m_fAccTime / m_fDashTime;

    if (m_fAccTime >= m_fDashTime)
    {
        ChangeState(DashEffectState::Progress);
        fRatio = 1.f;
    }

    if (nullptr != pRender)
    {
        m_fTextureValue += DT * m_fTextureValueSpeed;
        pRender->GetMaterial(0)->SetScalarParam(FLOAT_0, m_fTextureValue);
        pRender->GetMaterial(0)->SetScalarParam(VEC2_3, Vec2(fRatio, 0.f));
    }
}

void CUIDashEffectScript::Progress()
{
    CMeshRender* pRender = MeshRender();

    m_fRatio = Lerp(m_fRatio, 1.f, m_fRatioSpeed);

    if (nullptr != pRender)
    {
        m_fTextureValue += DT * m_fTextureValueSpeed;
        pRender->GetMaterial(0)->SetScalarParam(FLOAT_0, m_fTextureValue);
        pRender->GetMaterial(0)->SetScalarParam(VEC2_3, Vec2(1.f, m_fRatio));
    }
}

void CUIDashEffectScript::Stop()
{
    GamePlayStatic::DestroyGameObject(GetOwner());
}

UINT CUIDashEffectScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fRatioSpeed, sizeof(float), 1, _File);
    fwrite(&m_fTextureValueSpeed, sizeof(float), 1, _File);
    fwrite(&m_fMaxThreshHold, sizeof(float), 1, _File);
    fwrite(&m_fDashTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CUIDashEffectScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fRatioSpeed, sizeof(float), 1, _File);
    fread(&m_fTextureValueSpeed, sizeof(float), 1, _File);
    fread(&m_fMaxThreshHold, sizeof(float), 1, _File);
    fread(&m_fDashTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}