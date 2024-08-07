#include "pch.h"
#include "CPushOutColliderScript.h"

#include "CPushOutScript.h"
#include "CPlayerMgr.h"

CPushOutColliderScript::CPushOutColliderScript()
    : CScript(PUSHOUTCOLLIDERSCRIPT)
    , m_pParent(nullptr)
    , m_vBase{}
    , m_vDest{}
    , m_vDir{}
    , m_eState(PushOutColliderState::Stop)
    , m_fSpeed(0.f)
    , m_fReturnSpeed(0.f)
    , m_fOffset(1.f)
    , m_bFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDest, "Destination");
}

CPushOutColliderScript::CPushOutColliderScript(const CPushOutColliderScript& Origin)
    : CScript(Origin)
    , m_pParent(nullptr)
    , m_vBase{}
    , m_vDest{}
    , m_vDir{}
    , m_eState(PushOutColliderState::Stop)
    , m_fSpeed(Origin.m_fSpeed)
    , m_fReturnSpeed(Origin.m_fReturnSpeed)
    , m_fOffset(Origin.m_fOffset)
    , m_bFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDest, "Destination");
}

CPushOutColliderScript::~CPushOutColliderScript()
{
}

void CPushOutColliderScript::begin()
{
    m_vBase = Transform()->GetWorldPos();
}

void CPushOutColliderScript::tick()
{
    switch (m_eState)
    {
    case PushOutColliderState::MoveBase: {
        MoveBase();
    }
    break;
    case PushOutColliderState::MoveDest: {
        MoveDest();
    }
    break;
    case PushOutColliderState::Stop: {
        Stop();
    }
    break;
    case PushOutColliderState::End:
        break;
    default:
        break;
    }
}

void CPushOutColliderScript::MoveBase()
{
    Move(m_vDir * -1.f, m_vBase, m_fReturnSpeed);
}

void CPushOutColliderScript::MoveDest()
{
    Move(m_vDir, m_vDest, m_fSpeed);
}

void CPushOutColliderScript::Stop()
{
    Transform()->SetWorldPos(m_vBase);
}

void CPushOutColliderScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Body Collider" == pObj->GetName())
    {
        m_pParent->GetScript<CPushOutScript>()->SetPlayer(PLAYER);
    }
}

void CPushOutColliderScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Body Collider" == pObj->GetName())
    {
        m_pParent->GetScript<CPushOutScript>()->SetPlayer(nullptr);
    }
}

void CPushOutColliderScript::Move(Vec3 _vDir, Vec3 _vDest, float _fSpeed)
{
    Transform()->SetWorldPos(Transform()->GetWorldPos() + _vDir * _fSpeed * DT);
}

UINT CPushOutColliderScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_vDest, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(Vec3);

    return MemoryByte;
}

UINT CPushOutColliderScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_vDest, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(Vec3);

    return MemoryByte;
}