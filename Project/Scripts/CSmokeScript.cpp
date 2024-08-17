#include "pch.h"
#include "CSmokeScript.h"

CSmokeScript::CSmokeScript()
    : CScript(SMOKESCRIPT)
    , m_bRandom(false)
    , m_Acc(0.f)
    , m_LifeTime(1.f)
    , m_MaxLifeTime(1.f)
    , m_MaxSpeed(10.f)
    , m_MaxScale(20.f)
    , m_Speed(10.f)
    , m_Scale(20.f)
    , m_Dir{}
    , m_FadeOutAlpha(0.5f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_LifeTime, "Life Time", 0.1f);
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_Dir, "Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Scale, "Scale");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRandom, "Is Random?");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxLifeTime, "Max LifeTime", 0.1f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxSpeed, "Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxScale, "Max Scale");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FadeOutAlpha, "FadeOut Alpha(0~1)", 0.1f);
}

CSmokeScript::CSmokeScript(const CSmokeScript& _Origin)
    : CScript(SMOKESCRIPT)
    , m_bRandom(_Origin.m_bRandom)
    , m_Acc(0.f)
    , m_LifeTime(_Origin.m_LifeTime)
    , m_MaxLifeTime(_Origin.m_MaxLifeTime)
    , m_MaxSpeed(_Origin.m_MaxSpeed)
    , m_MaxScale(_Origin.m_MaxScale)
    , m_Speed(_Origin.m_Speed)
    , m_Scale(_Origin.m_Scale)
    , m_Dir(_Origin.m_Dir)
    , m_FadeOutAlpha(_Origin.m_FadeOutAlpha)
{

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_LifeTime, "Life Time", 0.1f);
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_Dir, "Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Scale, "Scale");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRandom, "Is Random?");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxLifeTime, "Max LifeTime", 0.1f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxSpeed, "Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxScale, "Max Scale");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FadeOutAlpha, "FadeOut Alpha(0~1)", 0.1f);
}

CSmokeScript::~CSmokeScript()
{
}

float simpleNoise(float min, float max)
{
    float noise = rand() / float(RAND_MAX);
    return min + (max - min) * noise;
}

void CSmokeScript::begin()
{
    m_Acc = 0.f;

    if (m_bRandom)
    {
        // Random으로 Scale, LifeTime, 방향을 정해준다.
        m_LifeTime = simpleNoise(0.5f, m_MaxLifeTime);
        m_Speed = simpleNoise(0.f, m_MaxSpeed);
        m_Scale = simpleNoise(5.f, m_MaxScale);
        m_Dir = Vec3(simpleNoise(-1.f, 1.f), simpleNoise(0.f, 1.f), simpleNoise(-1.f, 1.f));

        Vec3 Rot = Vec3(simpleNoise(0.f, 2.f * XM_PI), simpleNoise(0.f, 2.f * XM_PI), simpleNoise(0.f, 2.f * XM_PI));
        Transform()->SetLocalRotation(Rot);
    }

    m_Dir.Normalize();
}

void CSmokeScript::tick()
{
    m_Acc += DT;

    if (m_Acc < m_LifeTime)
    {
        float Ratio = m_Acc / m_LifeTime;

        float Alpha;
        float CurScale;
        float CurSpeed;

        // m_FadeOutAlpha 까지는 크기가 커진다.
        if (Ratio < m_FadeOutAlpha)
        {
            Ratio = Ratio / m_FadeOutAlpha;
            Alpha = sinf(Ratio * XM_PI * 0.5f);

            CurScale = m_Scale * Alpha;
            CurSpeed = m_Speed * Alpha;
        }
        else
        {
            Ratio = (Ratio - m_FadeOutAlpha) / (1.f - m_FadeOutAlpha);
            Alpha = cosf(Ratio * XM_PI * 0.5f);

            CurScale = m_Scale * Alpha;
            CurSpeed = m_Speed * Alpha;
        }

        // Pos
        Vec3 CurPos = Transform()->GetWorldPos();
        CurPos += CurSpeed * m_Dir * DT;
        Transform()->SetWorldPos(CurPos);

        // Scale
        Transform()->SetLocalScale(Vec3(CurScale, CurScale, CurScale));
    }
    // Life Time이 지나면 삭제
    else
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

UINT CSmokeScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_LifeTime, sizeof(float), 1, _File);
    fwrite(&m_Dir, sizeof(Vec3), 1, _File);
    fwrite(&m_Speed, sizeof(float), 1, _File);
    fwrite(&m_Scale, sizeof(float), 1, _File);
    fwrite(&m_bRandom, sizeof(bool), 1, _File);
    fwrite(&m_MaxLifeTime, sizeof(float), 1, _File);
    fwrite(&m_MaxSpeed, sizeof(float), 1, _File);
    fwrite(&m_MaxScale, sizeof(float), 1, _File);
    fwrite(&m_FadeOutAlpha, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CSmokeScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_LifeTime, sizeof(float), 1, _File);
    fread(&m_Dir, sizeof(Vec3), 1, _File);
    fread(&m_Speed, sizeof(float), 1, _File);
    fread(&m_Scale, sizeof(float), 1, _File);
    fread(&m_bRandom, sizeof(bool), 1, _File);
    fread(&m_MaxLifeTime, sizeof(float), 1, _File);
    fread(&m_MaxSpeed, sizeof(float), 1, _File);
    fread(&m_MaxScale, sizeof(float), 1, _File);
    fread(&m_FadeOutAlpha, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
