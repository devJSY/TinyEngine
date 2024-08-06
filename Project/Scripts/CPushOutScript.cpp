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
    , m_iChildNum{-1, -1, -1}
    , m_eState(PushOutState::Stop)
    , m_fSpeed(0.f)
    , m_fMoveBaseOffset(0.f)
    , m_fMoveDestOffset(0.f)
    , m_fReturnSpeed(0.f)
    , m_fOffset(1.f)
    , m_bFlag(false)
{
    for (int i = 0; i < 3; i++)
    {
        AddScriptParam(SCRIPT_PARAM::INT, &m_iChildNum[i], "PushOutColliderNum" + std::to_string(i));
    }

    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDest, "Destination");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fReturnSpeed, "Return Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fOffset, "Offset");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMoveBaseOffset, "MoveBaseOffset");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMoveDestOffset, "MoveDestOffset");
}

CPushOutScript::CPushOutScript(const CPushOutScript& Origin)
    : CScript(Origin)
    , m_pChild{}
    , m_pPlayer(nullptr)
    , m_vBase{}
    , m_vDest{}
    , m_vDir{}
    , m_iChildNum{-1, -1, -1}
    , m_eState(PushOutState::Stop)
    , m_fSpeed(Origin.m_fSpeed)
    , m_fMoveBaseOffset(Origin.m_fMoveBaseOffset)
    , m_fMoveDestOffset(Origin.m_fMoveDestOffset)
    , m_fReturnSpeed(Origin.m_fReturnSpeed)
    , m_fOffset(Origin.m_fOffset)
    , m_bFlag(false)
{
    for (int i = 0; i < 3; i++)
    {
        AddScriptParam(SCRIPT_PARAM::INT, &m_iChildNum[i], "PushOutColliderNum" + std::to_string(i));
    }

    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDest, "Destination");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fReturnSpeed, "Return Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fOffset, "Offset");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMoveBaseOffset, "MoveBaseOffset");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMoveDestOffset, "MoveDestOffset");
}

CPushOutScript::~CPushOutScript()
{
}

void CPushOutScript::begin()
{
    for (int i = 0; i < 3; i++)
    {
        if (-1 != m_iChildNum[i])
        {
            m_pChild[i] = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"PushOutColliderObj" + std::to_wstring(m_iChildNum[i]));
            m_pChild[i]->GetScript<CPushOutColliderScript>()->SetParent(GetOwner());
            m_pChild[i]->GetScript<CPushOutColliderScript>()->SetDestSpeed(m_fSpeed);
            m_pChild[i]->GetScript<CPushOutColliderScript>()->SetBaseSpeed(m_fReturnSpeed);
            m_pChild[i]->GetScript<CPushOutColliderScript>()->SetDir(m_vDir);
            m_pChild[i]->GetScript<CPushOutColliderScript>()->SetOffset(m_fOffset);
        }
    }
    m_vBase = Transform()->GetWorldPos();
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
    case PushOutState::End:
        break;
    default:
        break;
    }
}

void CPushOutScript::ChangeState(PushOutState _state)
{
    ExitState();
    m_eState = _state;
    EnterState();
}

void CPushOutScript::EnterState()
{
    switch (m_eState)
    {
    case PushOutState::MoveDest: {
        for (int i = 0;i < 3;i++)
        {
            if (nullptr != m_pChild[i])
            {
                m_pChild[i]->GetScript<CPushOutColliderScript>()->SetState(PushOutColliderState::MoveDest);
            }
        }
    }
    break;
    }
}

void CPushOutScript::MoveBase()
{
    Move(m_vDir * -1.f, m_vBase, m_fReturnSpeed, false);
}

void CPushOutScript::MoveDest()
{
    Move(m_vDir, m_vDest, m_fSpeed, true);
}

void CPushOutScript::Move(Vec3 _vDir, Vec3 _vDest, float _fSpeed, bool _flag)
{
    Vec3 vPos = Transform()->GetWorldPos();

    if ((_vDest.x - m_fOffset <= vPos.x && vPos.x <= _vDest.x + m_fOffset) && (_vDest.y - m_fOffset <= vPos.y && vPos.y <= _vDest.y + m_fOffset) &&
        (_vDest.z - m_fOffset <= vPos.z && vPos.z <= _vDest.z + m_fOffset))
    {
        m_eState = true == _flag ? PushOutState::MoveBase : PushOutState::Stop;
        Transform()->SetWorldPos(_vDest);
    }
    else
    {
        Transform()->SetWorldPos(Transform()->GetWorldPos() + _vDir * _fSpeed * DT);
    }

    if (nullptr != m_pPlayer)
    {
        float Offset = true == m_bFlag ? m_fMoveBaseOffset : m_fMoveDestOffset;
        m_pPlayer->CharacterController()->Move(_vDir * (_fSpeed / Offset) * DT);
    }
}

UINT CPushOutScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    for (int i = 0;i < 3;i++)
    {
        fwrite(&m_iChildNum[i], sizeof(int), 1, _File);
        MemoryByte += sizeof(int);
    }

    fwrite(&m_vDest, sizeof(Vec3), 1, _File);
    fwrite(&m_vDir, sizeof(Vec3), 1, _File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);
    fwrite(&m_fReturnSpeed, sizeof(float), 1, _File);
    fwrite(&m_fOffset, sizeof(float), 1, _File);
    fwrite(&m_fMoveBaseOffset, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CPushOutScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

     for (int i = 0; i < 3; i++)
    {
        fread(&m_iChildNum[i], sizeof(int), 1, _File);
        MemoryByte += sizeof(int);
    }

    fread(&m_vDest, sizeof(Vec3), 1, _File);
    fread(&m_vDir, sizeof(Vec3), 1, _File);
    fread(&m_fSpeed, sizeof(float), 1, _File);
    fread(&m_fReturnSpeed, sizeof(float), 1, _File);
    fread(&m_fOffset, sizeof(float), 1, _File);
    fread(&m_fMoveBaseOffset, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}