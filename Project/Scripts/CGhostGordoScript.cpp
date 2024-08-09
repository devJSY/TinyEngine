#include "pch.h"
#include "CGhostGordoScript.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CGhostGordoScript::CGhostGordoScript()
    : CMonsterUnitScript(GHOSTGORDOSCRIPT)
    , m_eState(GHOSTGORDO_STATE::EyeCloseWait)
    , m_qBaseQuat{}
    , m_vBasePos{}
    , m_fAccTime(0.f)
{
    SetEatable(false);
}

CGhostGordoScript::CGhostGordoScript(const CGhostGordoScript& _Origin)
    : CMonsterUnitScript(_Origin)
    , m_eState(GHOSTGORDO_STATE::EyeCloseWait)
    , m_qBaseQuat{}
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
    m_vBasePos = Transform()->GetWorldPos();
    m_qBaseQuat = Transform()->GetWorldQuaternion();
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
    case GHOSTGORDO_STATE::ReturnRotating: {
        ReturnRotating();
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
    case GHOSTGORDO_STATE::Track: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case GHOSTGORDO_STATE::Return: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
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
    CGameObject* pObj = GetTarget();
    if (pObj && PLAYERFSM->IsAttackEvent())
    {
        ChangeState(GHOSTGORDO_STATE::OpenEye);
    }
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
    (nullptr != GetTarget() && PLAYERFSM->IsAttackEvent()) ? Move(PLAYER->Transform()->GetWorldPos()) : ChangeState(GHOSTGORDO_STATE::TrackAfter1);
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
    Vec3 vPos = Transform()->GetWorldPos();
    (nullptr != GetTarget() && PLAYERFSM->IsAttackEvent()) ? ChangeState(GHOSTGORDO_STATE::Find) : Move(m_vBasePos);

    if ((m_vBasePos.x - 5.f <= vPos.x && vPos.x <= m_vBasePos.x + 5.f) && (m_vBasePos.y - 5.f <= vPos.y && vPos.x <= m_vBasePos.y + 5.f) &&
        (m_vBasePos.z - 5.f <= vPos.z && vPos.z <= m_vBasePos.z + 5.f))
    {
        ChangeState(GHOSTGORDO_STATE::ReturnRotating);
    }
}

void CGhostGordoScript::ReturnRotating()
{
    Quat qWorldQuat = Transform()->GetWorldQuaternion();

    Quat qChangeQuat = Quat::Slerp(m_qBaseQuat, qWorldQuat, GetCurInfo().RotationSpeed * DT);

    Transform()->SetWorldRotation(qChangeQuat);

    if ((qChangeQuat.x <= 0.001f && qChangeQuat.x >= -0.001f) || (qChangeQuat.z <= 0.001f && qChangeQuat.z >= -0.001f))
    {
        ChangeState(GHOSTGORDO_STATE::EyeCloseStart);
    }
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

void CGhostGordoScript::Move(Vec3 _vTraget)
{
    // 1. GhostGordo는 Y 축으로만 방향으로만 바라봄
    Quat qWolrdQuat = Transform()->GetWorldQuaternion();

    Vec3 vDir = _vTraget - Transform()->GetWorldPos();
    Vec3 vMoveDir = vDir.Normalize();

    vDir.y = 0.f;

    Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 vUP = vFront == Vec3(0.f, 0.f, -1.f) ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    Quat qQuat = Quat::LookRotation(-vDir, vUP);

    Quat qLerp = Quat::Slerp(qWolrdQuat, qQuat, GetCurInfo().RotationSpeed * DT);

    Transform()->SetWorldRotation(qLerp);

    // 2. GhostGordo는 Y 축으로도 이동함
    Rigidbody()->SetVelocity(vMoveDir * GetCurInfo().Speed * DT);
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
