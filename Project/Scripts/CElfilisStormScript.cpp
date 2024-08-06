#include "pch.h"
#include "CElfilisStormScript.h"

CElfilisStormScript::CElfilisStormScript()
    : CScript(ELFILISSTORMSCRIPT)
    , m_AccTime(0.f)
    , m_PlayTime(3.f)
    , m_StormIdx(0)
    , m_StormWhite(nullptr)
    , m_StormRed(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_StormIdx, "Storm Idx (0~6)");
}

CElfilisStormScript::CElfilisStormScript(const CElfilisStormScript& _Origin)
    : CScript(_Origin)
    , m_OriginScale(_Origin.m_OriginScale)
    , m_AccTime(0)
    , m_PlayTime(3.f)
    , m_StormIdx(_Origin.m_StormIdx)
    , m_StormWhite(nullptr)
    , m_StormRed(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_StormIdx, "Storm Idx (0~6)");
}

CElfilisStormScript::~CElfilisStormScript()
{
}

void CElfilisStormScript::begin()
{
    m_OriginScale = Transform()->GetLocalScale();
    m_StormRed = GetOwner()->GetChildObject(L"Tornado_Red");
    m_StormWhite = GetOwner()->GetChildObject(L"Tornado_White");
}

void CElfilisStormScript::tick()
{
    m_AccTime += DT;

    // Scaling
    static float SpawnTime = 0.2f;

    if (m_AccTime <= SpawnTime)
    {
        float t = m_AccTime / SpawnTime;
        Transform()->SetLocalScale(m_OriginScale * t);
    }

    // Rotation
    if (m_StormRed && m_StormWhite)
    {
        Vec3 NewRot = m_StormRed->Transform()->GetLocalRotation();
        NewRot.y -= XMConvertToRadians(360.f * 4.f * DT);
        m_StormRed->Transform()->SetLocalRotation(NewRot);
        
        NewRot = m_StormWhite->Transform()->GetLocalRotation();
        NewRot.y -= XMConvertToRadians(360.f * 8.f * DT);
        m_StormWhite->Transform()->SetLocalRotation(NewRot);
    }

    // Move
    float Angle = m_StormIdx * (2.f * XM_PI / 7.f);
    Angle += 0.2f * m_AccTime;
    
    float Delta0 = cosf(Angle) * (300.f * m_AccTime);
    float Delta1 = sinf(Angle) * (300.f * m_AccTime);
    Transform()->SetWorldPos(m_CenterPosition + Vec3(Delta0, 0.f, Delta1));

    // Destroy
    if (m_AccTime > m_PlayTime)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

UINT CElfilisStormScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_StormIdx, sizeof(int), 1, _File);
    MemoryByte += sizeof(int);

    return MemoryByte;
}

UINT CElfilisStormScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_StormIdx, sizeof(int), 1, _File);
    MemoryByte += sizeof(int);

    return MemoryByte;
}