#include "pch.h"
#include "CSmokeScript.h"

CSmokeScript::CSmokeScript()
    : CScript(SMOKESCRIPT)
    , m_bRandom(false)
    , m_bDirRandom(false)
    , m_bRotRandom(false)
    , m_Acc(0.f)
    , m_LifeTime(1.f)
    , m_Speed(10.f)
    , m_Scale(20.f)
    , m_MinLifeTime(0.5f)
    , m_MaxLifeTime(1.f)
    , m_MinSpeed(10.f)
    , m_MaxSpeed(15.f)
    , m_MinScale(10.f)
    , m_MaxScale(20.f)
    , m_Dir{}
    , m_FadeOutAlpha(0.5f)
    , m_bHorizontalDir(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_LifeTime, "Life Time", 0.1f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Scale, "Scale");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRandom, "Is Random?");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bDirRandom, "Is Dir Random?");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRotRandom, "Is Rot Random?");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinLifeTime, "Min LifeTime", 0.1f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxLifeTime, "Max LifeTime", 0.1f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinSpeed, "Min Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxSpeed, "Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinScale, "Min Scale");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxScale, "Max Scale");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FadeOutAlpha, "FadeOut Alpha(0~1)", 0.1f);
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bHorizontalDir, "Is Rot Horizontal?");
}

CSmokeScript::CSmokeScript(const CSmokeScript& _Origin)
    : CScript(SMOKESCRIPT)
    , m_bRandom(_Origin.m_bRandom)
    , m_bDirRandom(_Origin.m_bDirRandom)
    , m_bRotRandom(_Origin.m_bRotRandom)
    , m_Acc(0.f)
    , m_LifeTime(_Origin.m_LifeTime)
    , m_MinLifeTime(_Origin.m_MinLifeTime)
    , m_MaxLifeTime(_Origin.m_MaxLifeTime)
    , m_MinSpeed(_Origin.m_MinSpeed)
    , m_MaxSpeed(_Origin.m_MaxSpeed)
    , m_MinScale(_Origin.m_MinScale)
    , m_MaxScale(_Origin.m_MaxScale)
    , m_Speed(_Origin.m_Speed)
    , m_Scale(_Origin.m_Scale)
    , m_Dir(_Origin.m_Dir)
    , m_FadeOutAlpha(_Origin.m_FadeOutAlpha)
    , m_bHorizontalDir(_Origin.m_bHorizontalDir)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_LifeTime, "Life Time", 0.1f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Scale, "Scale");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRandom, "Is Random?");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bDirRandom, "Is Dir Random?");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRotRandom, "Is Rot Random?");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinLifeTime, "Min LifeTime", 0.1f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxLifeTime, "Max LifeTime", 0.1f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinSpeed, "Min Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxSpeed, "Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinScale, "Min Scale");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxScale, "Max Scale");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FadeOutAlpha, "FadeOut Alpha(0~1)", 0.1f);
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bHorizontalDir, "Is Rot Horizontal?");
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
        m_LifeTime = simpleNoise(m_MinLifeTime, m_MaxLifeTime);
        m_Speed = simpleNoise(m_MinSpeed, m_MaxSpeed);
        m_Scale = simpleNoise(m_MinScale, m_MaxScale);

    }

    if (m_bRotRandom)
    {
        Vec3 Rot = Vec3(simpleNoise(0.f, 2.f * XM_PI), simpleNoise(0.f, 2.f * XM_PI), simpleNoise(0.f, 2.f * XM_PI));
        Transform()->SetLocalRotation(Rot);
    }

    if (m_bDirRandom)
    {
        m_Dir = Vec3(simpleNoise(-1.f, 1.f), simpleNoise(0.f, 1.f), simpleNoise(-1.f, 1.f));
    }
    else if (m_bHorizontalDir)
    {
        m_Dir = Vec3(simpleNoise(-1.f, 1.f), 0.f, simpleNoise(-1.f, 1.f));
        m_Dir.Normalize();
        m_Dir.y += simpleNoise(0.f, 0.3f);
        m_Dir.Normalize();
    }
    else 
    {
        m_Dir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
        m_Dir.y += simpleNoise(0.f, 0.6f);
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
    fwrite(&m_Speed, sizeof(float), 1, _File);
    fwrite(&m_Scale, sizeof(float), 1, _File);
    fwrite(&m_bRandom, sizeof(bool), 1, _File);
    fwrite(&m_bDirRandom, sizeof(bool), 1, _File);
    fwrite(&m_bRotRandom, sizeof(bool), 1, _File);
    fwrite(&m_MinLifeTime, sizeof(float), 1, _File);
    fwrite(&m_MaxLifeTime, sizeof(float), 1, _File);
    fwrite(&m_MinSpeed, sizeof(float), 1, _File);
    fwrite(&m_MaxSpeed, sizeof(float), 1, _File);
    fwrite(&m_MinScale, sizeof(float), 1, _File);
    fwrite(&m_MaxScale, sizeof(float), 1, _File);
    fwrite(&m_FadeOutAlpha, sizeof(float), 1, _File);
    fwrite(&m_bHorizontalDir, sizeof(bool), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CSmokeScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_LifeTime, sizeof(float), 1, _File);
    fread(&m_Speed, sizeof(float), 1, _File);
    fread(&m_Scale, sizeof(float), 1, _File);
    fread(&m_bRandom, sizeof(bool), 1, _File);
    fread(&m_bDirRandom, sizeof(bool), 1, _File);
    fread(&m_bRotRandom, sizeof(bool), 1, _File);
    fread(&m_MinLifeTime, sizeof(float), 1, _File);
    fread(&m_MaxLifeTime, sizeof(float), 1, _File);
    fread(&m_MinSpeed, sizeof(float), 1, _File);
    fread(&m_MaxSpeed, sizeof(float), 1, _File);
    fread(&m_MinScale, sizeof(float), 1, _File);
    fread(&m_MaxScale, sizeof(float), 1, _File);
    fread(&m_FadeOutAlpha, sizeof(float), 1, _File);
    fread(&m_bHorizontalDir, sizeof(bool), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}
