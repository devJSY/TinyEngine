#include "pch.h"
#include "CPushOutColliderScript.h"

#include "CPushOutScript.h"
#include "CPlayerMgr.h"

CPushOutColliderScript::CPushOutColliderScript()
    : CScript(PUSHOUTCOLLIDERSCRIPT)
    , m_pPlayer(nullptr)
    , m_vBase{}
    , m_vDir{}
    , m_eState(PushOutColliderState::Stop)
    , m_fSpeed(0.f)
    , m_fReturnSpeed(0.f)
    , m_bFlag(false)
{
}

CPushOutColliderScript::CPushOutColliderScript(const CPushOutColliderScript& Origin)
    : CScript(Origin)
    , m_pPlayer(nullptr)
    , m_vBase{}
    , m_vDir{}
    , m_eState(PushOutColliderState::Stop)
    , m_fSpeed(Origin.m_fSpeed)
    , m_fReturnSpeed(Origin.m_fReturnSpeed)
    , m_bFlag(false)
{
}

CPushOutColliderScript::~CPushOutColliderScript()
{
}

void CPushOutColliderScript::begin()
{
    BoxCollider()->SetEnabled(false);
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
    Move(m_vDir * -1.f, m_fReturnSpeed);
}

void CPushOutColliderScript::MoveDest()
{
    Move(m_vDir, m_fSpeed);
}

void CPushOutColliderScript::Stop()
{
    Transform()->SetWorldPos(m_vBase);
}

void CPushOutColliderScript::OnTriggerStay(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Body Collider" == pObj->GetName())
    {
        float fOffset = 1.f;
        Vec3 vWolrdPos = Transform()->GetWorldPos();
        Vec3 vWorldScale = Transform()->GetWorldScale();

        Vec3 vPlayerPos = pObj->GetParent()->Transform()->GetWorldPos();

        Vec2 LT = Vec2(vWolrdPos.x - vWorldScale.x, vWolrdPos.z - vWorldScale.z);
        Vec2 RB = Vec2(vWolrdPos.x + vWorldScale.x, vWolrdPos.z + vWorldScale.z);

        if ((LT.x + fOffset <= vPlayerPos.x && vPlayerPos.x <= RB.x - fOffset) &&
            (LT.y + fOffset <= vPlayerPos.z && vPlayerPos.z <= RB.y - fOffset) && m_eState == PushOutColliderState::MoveBase)
        {
            m_pPlayer = PLAYER;
        }   

        if (m_eState == PushOutColliderState::MoveDest)
        {
            m_pPlayer = PLAYER;
        }
    }
}

void CPushOutColliderScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Body Collider" == pObj->GetName())
    {
        m_pPlayer = nullptr;
    }
}

void CPushOutColliderScript::Move(Vec3 _vDir, float _fSpeed)
{
    Transform()->SetWorldPos(Transform()->GetWorldPos() + _vDir * _fSpeed * DT);
}

UINT CPushOutColliderScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CPushOutColliderScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}