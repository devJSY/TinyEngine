#include "pch.h"
#include "CPushOutScript.h"

#include "CPushOutColliderScript.h"

CPushOutScript::CPushOutScript()
    : CScript(PUSHOUTSCRIPT)
    , m_pChild{}
    , m_pPlayer(nullptr)
    , m_vBase{}
    , m_vDest{}
    , m_vDir{}
    , m_eState(PushOutState::Stop)
    , m_fSpeed(0.f)
    , m_fReturnSpeed(0.f)
    , m_fOffset(1.f)
    , m_bFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDest, "Destination");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fReturnSpeed, "Return Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fOffset, "Offset");
}

CPushOutScript::CPushOutScript(const CPushOutScript& Origin)
    : CScript(Origin)
    , m_pChild{}
    , m_pPlayer(nullptr)
    , m_vBase{}
    , m_vDest{}
    , m_vDir{}
    , m_eState(PushOutState::Stop)
    , m_fSpeed(Origin.m_fSpeed)
    , m_fReturnSpeed(Origin.m_fReturnSpeed)
    , m_fOffset(Origin.m_fOffset)
    , m_bFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDest, "Destination");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fReturnSpeed, "Return Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fOffset, "Offset");
}

CPushOutScript::~CPushOutScript()
{
}

void CPushOutScript::begin()
{
    m_vBase = Transform()->GetWorldPos();

    for (int i = 0; i < 3; i++)
    {
        m_pChild[i] = GetOwner()->GetParent()->GetChildObject(L"POC" + std::to_wstring(i));
        if (nullptr != m_pChild[i])
            m_pChild[i]->GetScript<CPushOutColliderScript>()->SetParent(GetOwner());
    }
}

void CPushOutScript::tick()
{
    switch (m_eState)
    {
    case PushOutState::MoveBase: {
        MoveBase();
    }
    break;
    case PushOutState::MoveDest: {
        MoveDest();
    }
    break;
    case PushOutState::Stop: {
        Stop();
    }
    break;
    case PushOutState::End:
        break;
    default:
        break;
    }
}

void CPushOutScript::MoveBase()
{
    MoveWithChild(-1.f * m_vDir, m_vBase, m_fReturnSpeed, false);

    if (m_bFlag)
    {
        m_bFlag = false;
        nullptr != m_pPlayer ? m_pPlayer->CharacterController()->Move(Vec3(0.f, 0.001f, 0.f)) : void();
    }
    else
    {
        m_bFlag = true;
        nullptr != m_pPlayer ? m_pPlayer->CharacterController()->Move(Vec3(0.f, -0.001f, 0.f)) : void();
    }

    nullptr != m_pPlayer ? m_pPlayer->CharacterController()->Move(-1.f * m_vDir * m_fReturnSpeed * DT) : void();
}

void CPushOutScript::MoveDest()
{
    MoveWithChild(m_vDir, m_vBase, m_fSpeed, true);

    if (m_bFlag)
    {
        m_bFlag = false;
        nullptr != m_pPlayer ? m_pPlayer->CharacterController()->Move(Vec3(0.f, 0.001f, 0.f)) : void();
    }
    else
    {
        m_bFlag = true;
        nullptr != m_pPlayer ? m_pPlayer->CharacterController()->Move(Vec3(0.f, -0.001f, 0.f)) : void();
    }

    nullptr != m_pPlayer ? m_pPlayer->CharacterController()->Move(m_vDir * m_fSpeed * DT) : void();
}

void CPushOutScript::Stop()
{
    MoveWithChild(Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 0.f), 0.f, false);
}

void CPushOutScript::MoveWithChild(Vec3 _vDir, Vec3 _vDest, float _fSpeed, bool _flag)
{
    Vec3 vPos = Transform()->GetWorldPos();

    if ((m_vDest.x - m_fOffset <= vPos.x && vPos.x <= m_vDest.x + m_fOffset) &&
        (m_vDest.y - m_fOffset <= vPos.y && vPos.y <= m_vDest.y + m_fOffset) && (m_vDest.z - m_fOffset <= vPos.z && vPos.z <= m_vDest.z + m_fOffset))
    {
        m_eState = true == _flag ? PushOutState::MoveBase : PushOutState::Stop;
        Transform()->SetWorldPos(_vDest);
    }
    else
    {
        Rigidbody()->SetVelocity(m_vDir * _fSpeed * DT);

        for (int i = 0; i < 3; i++)
        {
            if (nullptr != m_pChild[i])
            {
                m_pChild[i]->Rigidbody()->SetVelocity(m_vDir * _fSpeed * DT);
            }
        }
    }
}

UINT CPushOutScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_vDest, sizeof(Vec3), 1, _File);
    fwrite(&m_vDir, sizeof(Vec3), 1, _File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);
    fwrite(&m_fReturnSpeed, sizeof(float), 1, _File);
    fwrite(&m_fOffset, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CPushOutScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_vDest, sizeof(Vec3), 1, _File);
    fread(&m_vDir, sizeof(Vec3), 1, _File);
    fread(&m_fSpeed, sizeof(float), 1, _File);
    fread(&m_fReturnSpeed, sizeof(float), 1, _File);
    fread(&m_fOffset, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}