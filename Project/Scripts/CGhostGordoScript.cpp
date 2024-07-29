#include "pch.h"
#include "CGhostGordoScript.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CGhostGordoScript::CGhostGordoScript()
    : CMonsterUnitScript(GHOSTGORDOSCRIPT)
    , m_eState(GHOSTGORDO_STATE::EyeCloseWait)
    , m_vBasePos{}
    , m_fAccTime(0.f)
{
    SetEatable(false);
}

CGhostGordoScript::CGhostGordoScript(const CGhostGordoScript& _Origin)
    : CMonsterUnitScript(_Origin)
    , m_eState(GHOSTGORDO_STATE::EyeCloseWait)
    , m_vBasePos{}
    , m_fAccTime(0.f)
{
    SetEatable(false);
}

CGhostGordoScript::~CGhostGordoScript()
{
}

void CGhostGordoScript::begin()
{
    ChangeState(GHOSTGORDO_STATE::EyeCloseWait);
}

void CGhostGordoScript::tick()
{
    switch (m_eState)
    {
    case GHOSTGORDO_STATE::EyeCloseWait: {
        EyeCloseWait();
    }
    break;
    case GHOSTGORDO_STATE::EyeOpenWait: {
        EyeOpenWait();
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
    case GHOSTGORDO_STATE::TrackAfter1: {
        TrackAfter1();
    }
    break;
    case GHOSTGORDO_STATE::Return: {
        Return();
    }
    break;
    case GHOSTGORDO_STATE::EyeCloseStart: {
        EyeCloseStart();
    }
    break;
    case GHOSTGORDO_STATE::TrackWait: {
    }
    break;
    case GHOSTGORDO_STATE::End:
        break;
    default:
        break;
    }
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
    case GHOSTGORDO_STATE::EyeCloseWait: {
        Animator()->Play(ANIMPREFIX("EyeCloseWait"), false);
    }
    break;
    case GHOSTGORDO_STATE::EyeOpenWait: {
        Animator()->Play(ANIMPREFIX("EyeOpenWait"), false);
    }
    break;
    case GHOSTGORDO_STATE::Find: {
        Animator()->Play(ANIMPREFIX("LowToHighSub"), false);
    }
    break;
    case GHOSTGORDO_STATE::Track: {
        Animator()->Play(ANIMPREFIX("EyeOpenWait"), false);
    }
    break;
    case GHOSTGORDO_STATE::OpenEye: {
        Animator()->Play(ANIMPREFIX("EyeOpenStart"), false);
    }
    break;
    case GHOSTGORDO_STATE::TrackAfter1: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        Animator()->Play(ANIMPREFIX("Look"), false);
    }
    break;
    case GHOSTGORDO_STATE::TrackAfter2: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        Animator()->Play(ANIMPREFIX("Look"), false);
    }
    break;
    case GHOSTGORDO_STATE::EyeCloseStart: {
        Animator()->Play(ANIMPREFIX("EyeCloseStart"), false, false, 2.5f, 0.5f);
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
    case GHOSTGORDO_STATE::TrackWait: {
        m_fAccTime = 0.f;
    }
    break;
    default:
        break;
    }
}

void CGhostGordoScript::EyeCloseWait()
{
    (nullptr != GetTarget() && PLAYERFSM->IsAttackEvent()) ? ChangeState(GHOSTGORDO_STATE::OpenEye) : void();
}

void CGhostGordoScript::EyeOpenWait()
{
    (nullptr != GetTarget() && PLAYERFSM->IsAttackEvent()) ? ChangeState(GHOSTGORDO_STATE::Find) : void();

    Animator()->IsFinish() ? ChangeState(GHOSTGORDO_STATE::Return) : void();
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
    RotatingToTarget();
    (nullptr != GetTarget() && PLAYERFSM->IsAttackEvent()) ? RigidbodyMove(GetTarget()) : ChangeState(GHOSTGORDO_STATE::TrackAfter1);
}

void CGhostGordoScript::TrackAfter1()
{
    Animator()->IsFinish() ? ChangeState(GHOSTGORDO_STATE::Return) : void();
}

void CGhostGordoScript::TrackAfter2()
{
    Animator()->IsFinish() ? ChangeState(GHOSTGORDO_STATE::TrackWait) : void();
}

void CGhostGordoScript::Return()
{
    Vec3 ToTargetDir = m_vBasePos - Transform()->GetWorldPos();
    Transform()->Slerp(ToTargetDir, DT * m_CurInfo.RotationSpeed);

    Vec3 vPos = Transform()->GetWorldPos();
    (nullptr != GetTarget() && PLAYERFSM->IsAttackEvent()) ? ChangeState(GHOSTGORDO_STATE::Find)
                                                           : Rigidbody()->SetVelocity(ToTargetDir * (m_CurInfo.Speed * 1.5f));

    (m_vBasePos.x - 1.f <= vPos.x && vPos.x <= m_vBasePos.x + 1.f) && (m_vBasePos.x - 1.f <= vPos.x && vPos.x <= m_vBasePos.x + 1.f)
        ? ChangeState(GHOSTGORDO_STATE::EyeCloseStart)
        : void();
}

void CGhostGordoScript::EyeCloseStart()
{
    Animator()->IsFinish() ? ChangeState(GHOSTGORDO_STATE::EyeCloseWait) : void();
}

void CGhostGordoScript::TrackWait()
{
    m_fAccTime += DT;
    m_fAccTime >= 2.f ? ChangeState(GHOSTGORDO_STATE::EyeOpenWait) : void();
}

UINT CGhostGordoScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CGhostGordoScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::LoadFromLevelFile(_File);

    return MemoryByte;
}

void CGhostGordoScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();

    if (LAYER_PLAYER == pObj->GetLayerIdx() && L"Main Player" == pObj->GetName())
    {
        UnitHit hitInfo = {DAMAGE_TYPE::NORMAL, Transform()->GetWorldDir(DIR_TYPE::FRONT), m_CurInfo.ATK, 0.f, 0.f};
        pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo);
        ChangeState(GHOSTGORDO_STATE::TrackAfter2);
    }
}

void CGhostGordoScript::OnTriggerExit(CCollider* _OtherCollider)
{
}
