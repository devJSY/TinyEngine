#include "pch.h"
#include "CBoomerangKnight.h"

CBoomerangKnight::CBoomerangKnight()
    : CMonsterUnitScript(BOOMERANGKNIGHT)
    , m_eState(BOOMERANGKNIGHT_STATE::Idle)
    , m_vDamageDir{}
{
}

CBoomerangKnight::CBoomerangKnight(const CBoomerangKnight& _Origin)
    : CMonsterUnitScript(_Origin)
    , m_eState(BOOMERANGKNIGHT_STATE::Idle)
    , m_vDamageDir{}
{
}

CBoomerangKnight::~CBoomerangKnight()
{
}

void CBoomerangKnight::begin()
{
    ChangeState(BOOMERANGKNIGHT_STATE::Idle);
}

void CBoomerangKnight::tick()
{
    switch (m_eState)
    {
    case BOOMERANGKNIGHT_STATE::Idle:
        break;
    case BOOMERANGKNIGHT_STATE::Find:
        break;
    case BOOMERANGKNIGHT_STATE::AirCutterJumpStart:
        break;
    case BOOMERANGKNIGHT_STATE::AirCutterJump:
        break;
    case BOOMERANGKNIGHT_STATE::AirCutterThrow:
        break;
    case BOOMERANGKNIGHT_STATE::CutterThrowStart:
        break;
    case BOOMERANGKNIGHT_STATE::CutterThrowStartWait:
        break;
    case BOOMERANGKNIGHT_STATE::CutterThrow:
        break;
    case BOOMERANGKNIGHT_STATE::CutterCatch:
        break;
    case BOOMERANGKNIGHT_STATE::Damage:
        break;
    case BOOMERANGKNIGHT_STATE::Death:
        break;
    case BOOMERANGKNIGHT_STATE::Land:
        break;
    case BOOMERANGKNIGHT_STATE::End:
        break;
    default:
        break;
    }
}

void CBoomerangKnight::EnterState(BOOMERANGKNIGHT_STATE _state)
{
    switch (m_eState)
    {
    case BOOMERANGKNIGHT_STATE::Idle:
        break;
    case BOOMERANGKNIGHT_STATE::Find:
        break;
    case BOOMERANGKNIGHT_STATE::AirCutterJumpStart:
        break;
    case BOOMERANGKNIGHT_STATE::AirCutterJump:
        break;
    case BOOMERANGKNIGHT_STATE::AirCutterThrow:
        break;
    case BOOMERANGKNIGHT_STATE::CutterThrowStart:
        break;
    case BOOMERANGKNIGHT_STATE::CutterThrowStartWait:
        break;
    case BOOMERANGKNIGHT_STATE::CutterThrow:
        break;
    case BOOMERANGKNIGHT_STATE::CutterCatch:
        break;
    case BOOMERANGKNIGHT_STATE::Damage:
        break;
    case BOOMERANGKNIGHT_STATE::Death:
        break;
    case BOOMERANGKNIGHT_STATE::Land:
        break;
    case BOOMERANGKNIGHT_STATE::End:
        break;
    default:
        break;
    }
}

void CBoomerangKnight::ChangeState(BOOMERANGKNIGHT_STATE _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CBoomerangKnight::ExitState(BOOMERANGKNIGHT_STATE _state)
{
    switch (m_eState)
    {
    case BOOMERANGKNIGHT_STATE::Idle:
        break;
    case BOOMERANGKNIGHT_STATE::Find:
        break;
    case BOOMERANGKNIGHT_STATE::AirCutterJumpStart:
        break;
    case BOOMERANGKNIGHT_STATE::AirCutterJump:
        break;
    case BOOMERANGKNIGHT_STATE::AirCutterThrow:
        break;
    case BOOMERANGKNIGHT_STATE::CutterThrowStart:
        break;
    case BOOMERANGKNIGHT_STATE::CutterThrowStartWait:
        break;
    case BOOMERANGKNIGHT_STATE::CutterThrow:
        break;
    case BOOMERANGKNIGHT_STATE::CutterCatch:
        break;
    case BOOMERANGKNIGHT_STATE::Damage:
        break;
    case BOOMERANGKNIGHT_STATE::Death:
        break;
    case BOOMERANGKNIGHT_STATE::Land:
        break;
    case BOOMERANGKNIGHT_STATE::End:
        break;
    default:
        break;
    }
}

void CBoomerangKnight::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);
}

void CBoomerangKnight::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
}

void CBoomerangKnight::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (BOOMERANGKNIGHT_STATE::Eaten == m_eState)
        return;

    CGameObject* pObj = _OtherCollider->GetOwner();

    UnitHit hit;
    ZeroMemory(&hit, sizeof(hit));

    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이고 있는 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(BOOMERANGKNIGHT_STATE::Eaten);
            m_vDamageDir = -pObj->GetComponent<CTransform>()->GetWorldDir(DIR_TYPE::FRONT);
            return;
        }

        pObj->GetScript<CUnitScript>()->GetDamage(GetHitInfo());
    }
}

void CBoomerangKnight::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이는게 끝난 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(BOOMERANGKNIGHT_STATE::Idle);
        }
    }
}

void CBoomerangKnight::Idle()
{
}

void CBoomerangKnight::Find()
{
}

void CBoomerangKnight::AirCutterJumpStart()
{
}

void CBoomerangKnight::AirCutterJump()
{
}

void CBoomerangKnight::AirCutterThrow()
{
}

void CBoomerangKnight::CutterThrowStart()
{
}

void CBoomerangKnight::CutterThrowStartWait()
{
}

void CBoomerangKnight::CutterThrow()
{
}

void CBoomerangKnight::CutterCatch()
{
}

void CBoomerangKnight::Damage()
{
}

void CBoomerangKnight::Death()
{
}

void CBoomerangKnight::Land()
{
}