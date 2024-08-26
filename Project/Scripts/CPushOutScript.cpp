#include "pch.h"
#include "CPushOutScript.h"

#include "CKirbyMoveController.h"
#include "CPushOutColliderScript.h"
#include "CPlayerMgr.h"

CPushOutScript::CPushOutScript()
    : CScript(PUSHOUTSCRIPT)
    , m_pChild{}
    , m_pPlayer(nullptr)
    , m_pPlayer1(nullptr)
    , m_vBase{}
    , m_vDest{}
    , m_vDir{}
    , m_iChildNum{-1, -1, -1}
    , m_eState(PushOutState::Stop)
    , m_fSpeed(0.f)
    , m_fMoveBaseOffset(0.f)
    , m_fMoveDestOffset(0.f)
    , m_fReturnSpeed(0.f)
    , m_bXMove(false)
    , m_bZMove(false)
    , m_bBasePlus(false)
    , m_bDestPlus(false)
{
    for (int i = 0; i < 3; i++)
    {
        AddScriptParam(SCRIPT_PARAM::INT, &m_iChildNum[i], "PushOutColliderNum" + std::to_string(i));
    }

    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDest, "Destination");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fReturnSpeed, "Return Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMoveBaseOffset, "MoveBaseOffset");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMoveDestOffset, "MoveDestOffset");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bXMove, "MoveX");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bZMove, "MoveZ");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bBasePlus, "BasePlus");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bDestPlus, "DestPlus");
}

CPushOutScript::CPushOutScript(const CPushOutScript& Origin)
    : CScript(Origin)
    , m_pChild{}
    , m_pPlayer(nullptr)
    , m_pPlayer1(nullptr)
    , m_vBase{}
    , m_vDest{}
    , m_vDir{}
    , m_iChildNum{-1, -1, -1}
    , m_eState(PushOutState::Stop)
    , m_fSpeed(Origin.m_fSpeed)
    , m_fMoveBaseOffset(Origin.m_fMoveBaseOffset)
    , m_fMoveDestOffset(Origin.m_fMoveDestOffset)
    , m_fReturnSpeed(Origin.m_fReturnSpeed)
    , m_bXMove(false)
    , m_bZMove(false)
    , m_bBasePlus(false)
    , m_bDestPlus(false)
{
    for (int i = 0; i < 3; i++)
    {
        AddScriptParam(SCRIPT_PARAM::INT, &m_iChildNum[i], "PushOutColliderNum" + std::to_string(i));
    }

    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDest, "Destination");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fReturnSpeed, "Return Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMoveBaseOffset, "MoveBaseOffset");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMoveDestOffset, "MoveDestOffset");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bXMove, "MoveX");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bZMove, "MoveZ");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bBasePlus, "BasePlus");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bDestPlus, "DestPlus");
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
            m_pChild[i]->GetScript<CPushOutColliderScript>()->SetDestSpeed(m_fSpeed);
            m_pChild[i]->GetScript<CPushOutColliderScript>()->SetBaseSpeed(m_fReturnSpeed);
            m_pChild[i]->GetScript<CPushOutColliderScript>()->SetDir(m_vDir);
        }
    }
    m_vBase = Transform()->GetWorldPos();
}

void CPushOutScript::tick()
{
    CheckPlayer();

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
        for (int i = 0; i < 3; i++)
        {
            if (nullptr != m_pChild[i])
            {
                m_pChild[i]->BoxCollider()->SetEnabled(true);
                m_pChild[i]->GetScript<CPushOutColliderScript>()->SetState(PushOutColliderState::MoveDest);
            }
        }
    }
    break;
    case PushOutState::MoveBase: {
        for (int i = 0; i < 3; i++)
        {
            if (nullptr != m_pChild[i])
            {
                m_pChild[i]->GetScript<CPushOutColliderScript>()->SetState(PushOutColliderState::MoveBase);
            }
        }
    }
    break;
    case PushOutState::Stop: {
        for (int i = 0; i < 3; i++)
        {
            if (nullptr != m_pChild[i])
            {
                m_pChild[i]->BoxCollider()->SetEnabled(false);
                m_pChild[i]->GetScript<CPushOutColliderScript>()->SetState(PushOutColliderState::Stop);
            }
        }
    }
    break;
    }
}

void CPushOutScript::MoveBase()
{
    if (m_bBasePlus)
    {
        if (m_bXMove)
        {
            MovePlusX(-1.f * m_vDir, m_vBase, m_fReturnSpeed, false);
        }
        else if (m_bZMove)
        {
            MovePlusZ(-1.f * m_vDir, m_vBase, m_fReturnSpeed, false);
        }
        else
        {
            ChangeState(PushOutState::Stop);
        }
    }
    else
    {
        if (m_bXMove)
        {
            MoveMinusX(-1.f * m_vDir, m_vBase, m_fReturnSpeed, false);
        }
        else if (m_bZMove)
        {
            MoveMinusZ(-1.f * m_vDir, m_vBase, m_fReturnSpeed, false);
        }
        else
        {
            ChangeState(PushOutState::Stop);
        }
    }
}

void CPushOutScript::MoveDest()
{
    if (m_bDestPlus)
    {
        if (m_bXMove)
        {
            MovePlusX(m_vDir, m_vDest, m_fSpeed, true);
        }
        else if (m_bZMove)
        {
            MovePlusZ(m_vDir, m_vDest, m_fSpeed, true);
        }
        else
        {
            ChangeState(PushOutState::Stop);
        }
    }
    else
    {
        if (m_bXMove)
        {
            MoveMinusX(m_vDir, m_vDest, m_fSpeed, true);
        }
        else if (m_bZMove)
        {
            MoveMinusZ(m_vDir, m_vDest, m_fSpeed, true);
        }
        else
        {
            ChangeState(PushOutState::Stop);
        }
    }
}

void CPushOutScript::Stop()
{
    Transform()->SetWorldPos(m_vBase);
}

void CPushOutScript::CheckPlayer()
{
    bool bFlag = false;
    for (int i = 0; i < 3; i++)
    {
        if (nullptr != m_pChild[i])
        {
            if (nullptr != m_pChild[i]->GetScript<CPushOutColliderScript>()->CheckPlayer())
            {
                bFlag = true;
            }
        }
    }

    if (bFlag)
    {
        m_pPlayer = PLAYER;
    }
    else
    {
        m_pPlayer = nullptr;
    }
}

void CPushOutScript::MovePlusX(Vec3 _vDir, Vec3 _vDest, float _fSpeed, bool _flag)
{
    Vec3 vPos = Transform()->GetWorldPos();

    if (_vDest.x <= vPos.x)
    {
        true == _flag ? ChangeState(PushOutState::MoveBase) : ChangeState(PushOutState::Stop);
    }
    else
    {
        Transform()->SetWorldPos(Transform()->GetWorldPos() + _vDir * _fSpeed * DT);
    }

    if (nullptr != m_pPlayer)
    {
        float Offset = m_fMoveBaseOffset;
        if (Offset == 0)
        {
            Offset = 30;
        }

        if (true == _flag)
        {
            PLAYERCTRL->PushAddMove((_vDir / Offset) * DT);
        }
        else
        {
            m_pPlayer->CharacterController()->Move((_vDir / Offset) * DT);
        }
    }
}

void CPushOutScript::MovePlusZ(Vec3 _vDir, Vec3 _vDest, float _fSpeed, bool _flag)
{
    Vec3 vPos = Transform()->GetWorldPos();

    if (_vDest.z <= vPos.z)
    {
        true == _flag ? ChangeState(PushOutState::MoveBase) : ChangeState(PushOutState::Stop);
    }
    else
    {
        Transform()->SetWorldPos(Transform()->GetWorldPos() + _vDir * _fSpeed * DT);
    }

    if (nullptr != m_pPlayer)
    {
        float Offset = m_fMoveBaseOffset;
        if (Offset == 0)
        {
            Offset = 30;
        }

        if (true == _flag)
        {
            PLAYERCTRL->PushAddMove((_vDir / Offset) * DT);
        }
        else
        {
            m_pPlayer->CharacterController()->Move((_vDir / Offset) * DT);
        }
    }
}

void CPushOutScript::MoveMinusX(Vec3 _vDir, Vec3 _vDest, float _fSpeed, bool _flag)
{
    Vec3 vPos = Transform()->GetWorldPos();

    if (_vDest.x >= vPos.x)
    {
        true == _flag ? ChangeState(PushOutState::MoveBase) : ChangeState(PushOutState::Stop);
    }
    else
    {
        Transform()->SetWorldPos(Transform()->GetWorldPos() + _vDir * _fSpeed * DT);
    }

    if (nullptr != m_pPlayer)
    {
        float Offset = m_fMoveBaseOffset;
        if (Offset == 0)
        {
            Offset = 30;
        }

        if (true == _flag)
        {
            PLAYERCTRL->PushAddMove((_vDir / Offset) * DT);
        }
        else
        {
            m_pPlayer->CharacterController()->Move((_vDir / Offset) * DT);
        }
    }
}

void CPushOutScript::MoveMinusZ(Vec3 _vDir, Vec3 _vDest, float _fSpeed, bool _flag)
{
    Vec3 vPos = Transform()->GetWorldPos();

    if (_vDest.z >= vPos.z)
    {
        true == _flag ? ChangeState(PushOutState::MoveBase) : ChangeState(PushOutState::Stop);
    }
    else
    {
        Transform()->SetWorldPos(Transform()->GetWorldPos() + _vDir * _fSpeed * DT);
    }

    if (nullptr != m_pPlayer)
    {
        float Offset = m_fMoveBaseOffset;
        if (Offset == 0)
        {
            Offset = 30;
        }

        if (true == _flag)
        {
            PLAYERCTRL->PushAddMove((_vDir / Offset) * DT);
        }
        else
        {
            m_pPlayer->CharacterController()->Move((_vDir / Offset) * DT);
        }
    }
}

UINT CPushOutScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    for (int i = 0; i < 3; i++)
    {
        fwrite(&m_iChildNum[i], sizeof(int), 1, _File);
        MemoryByte += sizeof(int);
    }

    fwrite(&m_vDest, sizeof(Vec3), 1, _File);
    fwrite(&m_vDir, sizeof(Vec3), 1, _File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);
    fwrite(&m_fReturnSpeed, sizeof(float), 1, _File);
    fwrite(&m_fMoveBaseOffset, sizeof(float), 1, _File);
    fwrite(&m_fMoveDestOffset, sizeof(float), 1, _File);

    fwrite(&m_bXMove, sizeof(bool), 1, _File);
    fwrite(&m_bZMove, sizeof(bool), 1, _File);

    fwrite(&m_bBasePlus, sizeof(bool), 1, _File);
    fwrite(&m_bDestPlus, sizeof(bool), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

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
    fread(&m_fMoveBaseOffset, sizeof(float), 1, _File);
    fread(&m_fMoveDestOffset, sizeof(float), 1, _File);

    fread(&m_bXMove, sizeof(bool), 1, _File);
    fread(&m_bZMove, sizeof(bool), 1, _File);

    fread(&m_bBasePlus, sizeof(bool), 1, _File);
    fread(&m_bDestPlus, sizeof(bool), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}