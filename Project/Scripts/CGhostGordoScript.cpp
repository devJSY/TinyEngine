#include "pch.h"
#include "CGhostGordoScript.h"

CGhostGordoScript::CGhostGordoScript()
    : CMonsterUnitScript(GHOSTGORDOSCRIPT)
    , m_eState(GHOSTGORDO_STATE::CloseEyeIdle)
{
}

CGhostGordoScript::CGhostGordoScript(const CGhostGordoScript& _Origin)
    : CMonsterUnitScript(_Origin)
    , m_eState(GHOSTGORDO_STATE::CloseEyeIdle)
{
}

CGhostGordoScript::~CGhostGordoScript()
{
}

void CGhostGordoScript::begin()
{
    ChangeState(GHOSTGORDO_STATE::CloseEyeIdle);
}

void CGhostGordoScript::tick()
{
    switch (m_eState)
    {
    case GHOSTGORDO_STATE::CloseEyeIdle: {
        CloseEyeIdle();
    }
    break;
    case GHOSTGORDO_STATE::OpenEyeIdle: {
        OpenEyeIdle();
    }
    break;
    case GHOSTGORDO_STATE::OpenEye: {
        OpenEye();
    }
    break;
    case GHOSTGORDO_STATE::Find: {
        Find();
    }
    break;
    case GHOSTGORDO_STATE::Track: {
        Track();
    }
    break;
    case GHOSTGORDO_STATE::TrackAfter: {
        TrackAfter();
    }
    break;
    case GHOSTGORDO_STATE::End:
        break;
    default:
        break;
    }
}

void CGhostGordoScript::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);
}

void CGhostGordoScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
}

void CGhostGordoScript::ChangeState(GHOSTGORDO_STATE _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CGhostGordoScript::EnterState(GHOSTGORDO_STATE _state)
{
    switch (_state)
    {
    case GHOSTGORDO_STATE::CloseEyeIdle: {
        Animator()->Play(ANIMPREFIX(L"EyeCloseWait"), false);
    }
    break;
    case GHOSTGORDO_STATE::OpenEyeIdle: {
        Animator()->Play(ANIMPREFIX(L"Look"));
    }
    break;
    case GHOSTGORDO_STATE::Find: {
        Animator()->Play(ANIMPREFIX(L"LowToHighSub"), false);
    }
    break;
    case GHOSTGORDO_STATE::Track: {
        Animator()->Play(ANIMPREFIX(L"EyeOpenWait"), false);
    }
    break;
    case GHOSTGORDO_STATE::OpenEye: {
        Animator()->Play(ANIMPREFIX(L"EyeOpenStart"), false);
    }
    break;
    case GHOSTGORDO_STATE::TrackAfter: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        Animator()->Play(ANIMPREFIX(L"Look"), false);
    }
    break;
    case GHOSTGORDO_STATE::End:
        break;
    default:
        break;
    }
}

void CGhostGordoScript::ExitState(GHOSTGORDO_STATE _state)
{
    switch (_state)
    {
    case GHOSTGORDO_STATE::OpenEyeIdle:
        break;
    case GHOSTGORDO_STATE::Track:
        break;
    case GHOSTGORDO_STATE::TrackAfter:
        break;
    case GHOSTGORDO_STATE::End:
        break;
    default:
        break;
    }
}

void CGhostGordoScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();

    LAYER_PLAYER == pObj->GetLayerIdx() ? ChangeState(GHOSTGORDO_STATE::OpenEyeIdle) : void();
}

void CGhostGordoScript::OnTriggerExit(CCollider* _OtherCollider)
{
}

void CGhostGordoScript::CloseEyeIdle()
{
    // TODO : ºû °¨Áö ±îÁö
    if (nullptr != GetTarget())
    {
        ChangeState(GHOSTGORDO_STATE::OpenEye);
    }
}

void CGhostGordoScript::OpenEyeIdle()
{
    Animator()->IsFinish() ? Animator()->Play(ANIMPREFIX(L"EyeOpenWait")) : void();
    
    // TODO : ºû °¨Áö ±îÁö
    if (nullptr != GetTarget())
    {
        ChangeState(GHOSTGORDO_STATE::Find);
    }
}

void CGhostGordoScript::OpenEye()
{
    Animator()->IsFinish() ? ChangeState(GHOSTGORDO_STATE::Find) : void();
}

void CGhostGordoScript::Find()
{
    Animator()->IsFinish() ? ChangeState(GHOSTGORDO_STATE::Track) : void();
}

void CGhostGordoScript::Track()
{
    TransformRotate();
    nullptr != GetTarget() ? RigidbodyMove(GetTarget()) : ChangeState(GHOSTGORDO_STATE::TrackAfter);
}

void CGhostGordoScript::TrackAfter()
{
    Animator()->IsFinish() ? ChangeState(GHOSTGORDO_STATE::OpenEyeIdle) : void();
}