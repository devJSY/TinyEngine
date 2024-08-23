#include "pch.h"
#include "CGhostGordoScript.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CGhostGordoScript::CGhostGordoScript()
    : CMonsterUnitScript(GHOSTGORDOSCRIPT)
    , m_eState(GhostGordoState::EyeCloseWait)
    , m_qBaseQuat{}
    , m_vBasePos{}
    , m_fAccTime(0.f)
{
    SetEatable(false);
}

CGhostGordoScript::CGhostGordoScript(const CGhostGordoScript& Origin)
    : CMonsterUnitScript(Origin)
    , m_eState(GhostGordoState::EyeCloseWait)
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
    CUnitScript::begin();

    InitSetting();

    ChangeState(GhostGordoState::EyeCloseWait);
}

void CGhostGordoScript::tick()
{
    FSM();
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

    if (LAYER_PLAYER_TRIGGER == pObj->GetLayerIdx() && L"Body Collider" == pObj->GetName())
    {
        UnitHit hitInfo = {DAMAGE_TYPE::NORMAL, Transform()->GetWorldDir(DIR_TYPE::FRONT), m_CurInfo.ATK, 0.f, 0.f};
        pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo);
        ChangeState(GhostGordoState::TrackAfter2);
    }
}

/////////////////////////// FUNC //////////////////////////////////
// 1. EnterState
// 2. FSM
// 3. ExitState
// 4. ChangeState
// 5. CheckDamage
// 6. Move

void CGhostGordoScript::InitSetting()
{
    m_vBasePos = Transform()->GetWorldPos();
    m_qBaseQuat = Transform()->GetWorldQuaternion();
}

void CGhostGordoScript::EnterState(GhostGordoState _state)
{
    switch (_state)
    {
    case GhostGordoState::EyeCloseWait: {
        Animator()->Play(ANIMPREFIX("EyeCloseWait"), false, false, 1.5f);
    }
    break;
    case GhostGordoState::EyeOpenWait: {
        Animator()->Play(ANIMPREFIX("EyeOpenWait"), false, false, 1.5f);
    }
    break;
    case GhostGordoState::Find: {
        Animator()->Play(ANIMPREFIX("LowToHighSub"), false, false, 1.5f);
    }
    break;
    case GhostGordoState::Track: {
        Animator()->Play(ANIMPREFIX("EyeOpenWait"), false, false, 1.5f);
    }
    break;
    case GhostGordoState::OpenEye: {
        Animator()->Play(ANIMPREFIX("EyeOpenStart"), false, false, 1.5f);
    }
    break;
    case GhostGordoState::TrackAfter1: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        Animator()->Play(ANIMPREFIX("Look"), false, false, 1.5f);
    }
    break;
    case GhostGordoState::TrackAfter2: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        Animator()->Play(ANIMPREFIX("Look"), false, false, 1.5f);
    }
    break;
    case GhostGordoState::EyeCloseStart: {
        Animator()->Play(ANIMPREFIX("EyeCloseStart"), false, false, 1.5f, 0.5f);
    }
    break;
    case GhostGordoState::End:
        break;
    default:
        break;
    }
}

void CGhostGordoScript::FSM()
{
    switch (m_eState)
    {
    case GhostGordoState::EyeCloseWait: {
        EyeCloseWait();
    }
    break;
    case GhostGordoState::EyeOpenWait: {
        EyeOpenWait();
    }
    break;
    case GhostGordoState::OpenEye: {
        OpenEye();
    }
    break;
    case GhostGordoState::Find: {
        Find();
    }
    break;
    case GhostGordoState::Track: {
        Track();
    }
    break;
    case GhostGordoState::TrackAfter1: {
        TrackAfter1();
    }
    break;
    case GhostGordoState::TrackAfter2: {
        TrackAfter2();
    }
    break;
    case GhostGordoState::Return: {
        Return();
    }
    break;
    case GhostGordoState::ReturnRotating: {
        ReturnRotating();
    }
    break;
    case GhostGordoState::EyeCloseStart: {
        EyeCloseStart();
    }
    break;
    case GhostGordoState::TrackWait: {
        TrackWait();
    }
    break;
    case GhostGordoState::End:
        break;
    default:
        break;
    }
}

void CGhostGordoScript::ExitState(GhostGordoState _state)
{
    switch (_state)
    {
    case GhostGordoState::Track: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case GhostGordoState::Return: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case GhostGordoState::TrackWait: {
        m_fAccTime = 0.f;
    }
    break;
    default:
        break;
    }
}

void CGhostGordoScript::ChangeState(GhostGordoState _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
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

/////////////////////////// FIND FSM //////////////////////////////////
// 1. EyeCloseWait
// 2. EyeOpenWait
// 3. OpenEye
// 4. Find
// 5. Track
// 6. TrackAfter1
// 7. TrackAfter2
// 8. EyeCloseStart
// 9. TrackWait
// 10. Return
// 11. ReturnRotating

#pragma region EYECLOSEWAIT
void CGhostGordoScript::EyeCloseWait()
{
    CGameObject* pObj = GetTarget();
    if (pObj && PLAYERFSM->IsAttackEvent())
    {
        ChangeState(GhostGordoState::OpenEye);
    }
}
#pragma endregion

#pragma region EYEOPENWAIT
void CGhostGordoScript::EyeOpenWait()
{
    (nullptr != GetTarget() && PLAYERFSM->IsAttackEvent()) ? ChangeState(GhostGordoState::Find) : void();

    Animator()->IsFinish() ? ChangeState(GhostGordoState::Return) : void();
}
#pragma endregion

#pragma region OPENEYE
void CGhostGordoScript::OpenEye()
{
    Animator()->IsFinish() ? ChangeState(GhostGordoState::Find) : void();
}
#pragma endregion

#pragma region FIND
void CGhostGordoScript::Find()
{
    Animator()->IsFinish() ? ChangeState(GhostGordoState::Track) : void();
}
#pragma endregion

#pragma region TRACK
void CGhostGordoScript::Track()
{
    (nullptr != GetTarget() && PLAYERFSM->IsAttackEvent()) ? Move(PLAYER->Transform()->GetWorldPos()) : ChangeState(GhostGordoState::TrackAfter1);
}
#pragma endregion

#pragma region TRACKAFTER1
void CGhostGordoScript::TrackAfter1()
{
    Animator()->IsFinish() ? ChangeState(GhostGordoState::Return) : void();
}
#pragma endregion

#pragma region TRACKAFTER2
void CGhostGordoScript::TrackAfter2()
{
    Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    Animator()->IsFinish() ? ChangeState(GhostGordoState::TrackWait) : void();
}
#pragma endregion

#pragma region RETURN
void CGhostGordoScript::Return()
{
    Vec3 vPos = Transform()->GetWorldPos();
    (nullptr != GetTarget() && PLAYERFSM->IsAttackEvent()) ? ChangeState(GhostGordoState::Find) : Move(m_vBasePos);

    if ((m_vBasePos.x - 5.f <= vPos.x && vPos.x <= m_vBasePos.x + 5.f) && (m_vBasePos.y - 5.f <= vPos.y && vPos.x <= m_vBasePos.y + 5.f) &&
        (m_vBasePos.z - 5.f <= vPos.z && vPos.z <= m_vBasePos.z + 5.f))
    {
        ChangeState(GhostGordoState::ReturnRotating);
    }
}
#pragma endregion

#pragma region RETURNROTATING
void CGhostGordoScript::ReturnRotating()
{
    Quat qWorldQuat = Transform()->GetWorldQuaternion();

    Quat qChangeQuat = Quat::Slerp(m_qBaseQuat, qWorldQuat, GetCurInfo().RotationSpeed * DT);

    Transform()->SetWorldRotation(qChangeQuat);

    if ((qChangeQuat.x <= 0.001f && qChangeQuat.x >= -0.001f) || (qChangeQuat.z <= 0.001f && qChangeQuat.z >= -0.001f))
    {
        ChangeState(GhostGordoState::EyeCloseStart);
    }
}
#pragma endregion

#pragma region EYECLOSESTART
void CGhostGordoScript::EyeCloseStart()
{
    Animator()->IsFinish() ? ChangeState(GhostGordoState::EyeCloseWait) : void();
}
#pragma endregion

#pragma region TRACKWAIT
void CGhostGordoScript::TrackWait()
{
    m_fAccTime += DT;
    m_fAccTime >= 2.f ? ChangeState(GhostGordoState::EyeOpenWait) : void();
}
#pragma endregion