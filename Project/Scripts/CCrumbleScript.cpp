#include "pch.h"
#include "CCrumbleScript.h"

#include "CPlayerMgr.h"

CCrumbleScript::CCrumbleScript()
    : CScript(CRUMBLESCRIPT)
    , m_pPlayer(nullptr)
    , m_eState(CrumbleState::Wait)
    , m_fAccTime(0.f)
    , m_fWaitTime(1.8f)
{
}

CCrumbleScript::CCrumbleScript(const CCrumbleScript& Origin)
    : CScript(Origin)
    , m_pPlayer(nullptr)
    , m_eState(CrumbleState::Wait)
    , m_fAccTime(0.f)
    , m_fWaitTime(1.8f)
{
}

CCrumbleScript::~CCrumbleScript()
{
}

void CCrumbleScript::begin()
{
    m_pPlayer = PLAYER;
    ChangeState(CrumbleState::Wait);
}

void CCrumbleScript::tick()
{
    switch (m_eState)
    {
    case CrumbleState::Wait: {
    }
    break;
    case CrumbleState::PreDisappear: {
        PreDisappear();
    }
    break;
    case CrumbleState::Disapper: {
        Disappear();
    }
    break;
    case CrumbleState::Appear: {
        Appear();
    }
    break;
    case CrumbleState::End:
        break;
    default:
        break;
    }
}

void CCrumbleScript::PreDisappear()
{
    m_fAccTime += DT;
    if (m_fAccTime >= m_fWaitTime)
    {
        ChangeState(CrumbleState::Disapper);
    }
}

void CCrumbleScript::Disappear()
{
    if (Animator()->IsFinish())
    {
        BoxCollider()->SetEnabled(false);

        m_fAccTime += DT;

        if (m_fAccTime >= m_fWaitTime)
            ChangeState(CrumbleState::Appear);
    }
}

void CCrumbleScript::Appear()
{
    if (Animator()->IsFinish())
    {
        BoxCollider()->SetEnabled(true);
        ChangeState(CrumbleState::Wait);
    }
}

void CCrumbleScript::ChangeState(CrumbleState _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CCrumbleScript::EnterState(CrumbleState _state)
{
    switch (_state)
    {
    case CrumbleState::Wait: {
        Animator()->Play(ANIMPREFIX("Wait"), false);
    }
    break;
    case CrumbleState::PreDisappear: {
        Animator()->Play(ANIMPREFIX("PreDisappear"), true, false, 2.5f);
    }
    break;
    case CrumbleState::Disapper: {
        Animator()->Play(ANIMPREFIX("Disappear"), false, false, 2.f);
    }
    break;
    case CrumbleState::Appear: {
        Animator()->Play(ANIMPREFIX("Appear"), false, false, 2.f);
    }
    break;
    case CrumbleState::End:
        break;
    default:
        break;
    }
}

void CCrumbleScript::ExitState(CrumbleState _state)
{
    switch (_state)
    {
    case CrumbleState::PreDisappear: {
        m_fAccTime = 0.f;
    }
    break;
    case CrumbleState::Disapper: {
        m_fAccTime = 0.f;
    }
    break;
    case CrumbleState::End:
        break;
    default:
        break;
    }
}

void CCrumbleScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    UINT Layer = _OtherCollider->GetOwner()->GetLayerIdx();
    CGameObject* Obj = _OtherCollider->GetOwner();
    if (Layer == LAYER_PLAYER_TRIGGER && L"Body Collider" == Obj->GetName())
    {
        ChangeState(CrumbleState::PreDisappear);
    }
}

UINT CCrumbleScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CCrumbleScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}