#include "pch.h"
#include "CKirbySmoke.h"

CKirbySmoke::CKirbySmoke()
    : CScript(KIRBYSMOKE)
    , m_SmokeOriginal(nullptr)
    , m_SmokeSplit(nullptr)
    , m_SmokeTail(nullptr)
    , m_Acc(0.f)
    , m_Speed(30.f)
    , m_SmokeType(0)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_SmokeType, "SmokeType 0: Run, 1: Spit");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
}

CKirbySmoke::CKirbySmoke(const CKirbySmoke& _Origin)
    : CScript(KIRBYSMOKE)
    , m_SmokeOriginal(nullptr)
    , m_SmokeSplit(nullptr)
    , m_SmokeTail(nullptr)
    , m_Acc(0.f)
    , m_Speed(_Origin.m_Speed)
    , m_SmokeType(_Origin.m_SmokeType)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_SmokeType, "SmokeType 0: Run, 1: Spit");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
}

CKirbySmoke::~CKirbySmoke()
{
}

void CKirbySmoke::begin()
{
    m_SmokeOriginal = GetOwner()->GetChildObject(L"SmokeSphereOriginal");
    m_SmokeSplit = GetOwner()->GetChildObject(L"SmokeSphereSplit");
    m_SmokeTail = GetOwner()->GetChildObject(L"SmokeTail");

    if (nullptr == m_SmokeOriginal || nullptr == m_SmokeSplit || nullptr == m_SmokeTail)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }

    m_Dir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    m_Acc = 0.f;
    m_MoveDuration = 0.4f;
    m_FadeDuration = 1.f;

    switch (m_SmokeType)
    {
    case (UINT)SmokeType::Run:
    {
        m_OriginalSize = 0.65f; 
        m_SplitSize = 0.15f;
        m_TailSize = 0.5f;
        m_Speed = 30.f;
    }

        break;

    case (UINT)SmokeType::Spit:
    {
        m_OriginalSize = 0.9f;
        m_SplitSize = 0.2f;
        m_TailSize = 0.4f;
    }
        break;

    default:
        break;
    }

}

void CKirbySmoke::tick()
{
    m_Acc += DT;

    if (m_Acc < m_MoveDuration)
    {
        float Ratio = clamp((m_Acc / m_MoveDuration), 0.f, 1.f);
        float CurSpeed = m_Speed * cosf(Ratio * XM_PI * 0.5f);

        Vec3 CurPos = Transform()->GetWorldPos();
        CurPos += CurSpeed * m_Dir * DT;

        Transform()->SetWorldPos(CurPos);

        // Tail은 점점 작아진다.
        float TailSize = cosf(Ratio * XM_PI * 0.5f) * m_TailSize; 
        m_SmokeTail->Transform()->SetLocalScale(Vec3(TailSize, TailSize, TailSize));

        // Original은 점점 커진다.
        float OriginalSize = sinf(Ratio * XM_PI * 0.5f) * m_OriginalSize;
        m_SmokeOriginal->Transform()->SetLocalScale(Vec3(OriginalSize, OriginalSize, OriginalSize));

        // Spit은 점점 커진다.
        float SpitSize = sinf(Ratio * XM_PI * 0.5f) * m_SplitSize;
        m_SmokeSplit->Transform()->SetLocalScale(Vec3(SpitSize, SpitSize, SpitSize));
    }
    else if (m_Acc < m_FadeDuration)
    {
        m_SmokeSplit->SetActive(true);

        float Ratio = clamp(((m_Acc - m_MoveDuration) / m_FadeDuration), 0.f, 1.f);

        // Original은 점점 작아진다.
        float OriginalSize = cosf(Ratio * XM_PI * 0.5f) * m_OriginalSize;
        m_SmokeOriginal->Transform()->SetLocalScale(Vec3(OriginalSize, OriginalSize, OriginalSize));

        // Spit은 점점 작아진다.
        float SpitSize = cosf(Ratio * XM_PI * 0.5f) * m_SplitSize;
        m_SmokeSplit->Transform()->SetLocalScale(Vec3(SpitSize, SpitSize, SpitSize));

        // 오브젝트는 점점 위로 올라간다.
        Vec3 CurPos = Transform()->GetWorldPos();
        
        CurPos += 5.f * m_Dir * DT;
        CurPos.y += 13.f * DT;

        Transform()->SetWorldPos(CurPos);
    }
    else
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

UINT CKirbySmoke::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_Speed, sizeof(float), 1, _File);
    fwrite(&m_SmokeType, sizeof(UINT), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(UINT);

    return MemoryByte;
}

UINT CKirbySmoke::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_Speed, sizeof(float), 1, _File);
    fread(&m_SmokeType, sizeof(UINT), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(UINT);

    return MemoryByte;
}

