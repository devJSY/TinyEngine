#include "pch.h"
#include "CKirbyFSM.h"
#include "CKirbyAbility.h"
#include "CKirbyObject.h"
#include "CKirbyVacuumCollider.h"

#include "CKirbyAbility_Normal.h"

CKirbyFSM::CKirbyFSM()
    : CFSMScript(KIRBYFSM)
    , m_arrAbility{}
    , m_arrObject{}
    , m_CurAbility(AbilityCopyType::NORMAL)
    , m_NextAbility(AbilityCopyType::NONE)
    , m_CurObject(ObjectCopyType::NONE)
    , m_StuffedCopyObj(nullptr)
    , m_VacuumCollider(nullptr)
    , m_ChargeAccTime(0.f)
    , m_HoveringAccTime(0.f)
    , m_HoveringLimitTime(7.f)
    , m_LastJump(LastJumpType::HIGH)
    , m_bStuffed(false)
    , m_bHovering(false)
    , m_bInvincible(false)
    , m_InvincibleAcc(0.f)
    , m_InvincibleDuration(3.f)
    , m_EmissiveCoef(0.f)
{
    // @TODO Copy Type마다 추가
    m_arrAbility[(UINT)AbilityCopyType::NORMAL] = new CKirbyAbility_Normal();
    m_arrAbility[(UINT)AbilityCopyType::FIRE] = new CKirbyAbility_Normal();
}

CKirbyFSM::CKirbyFSM(const CKirbyFSM& _Origin)
    : CFSMScript(_Origin)
    , m_arrAbility{}
    , m_arrObject{}
    , m_CurAbility(_Origin.m_CurAbility)
    , m_CurObject(_Origin.m_CurObject)
    , m_StuffedCopyObj(nullptr)
    , m_VacuumCollider(nullptr)
    , m_ChargeAccTime(0.f)
    , m_HoveringAccTime(0.f)
    , m_HoveringLimitTime(_Origin.m_HoveringLimitTime)
    , m_LastJump(LastJumpType::HIGH)
    , m_bStuffed(false)
    , m_bHovering(false)
    , m_bInvincible(false)
    , m_InvincibleAcc(_Origin.m_InvincibleAcc)
    , m_InvincibleDuration(_Origin.m_InvincibleDuration)
    , m_EmissiveCoef(_Origin.m_EmissiveCoef)
{
    // Ability Copy 복사
    for (UINT i = 0; i < (UINT)AbilityCopyType::END; ++i)
    {
        if (nullptr == _Origin.m_arrAbility[i])
            continue;

        CKirbyAbility* pAbil = _Origin.m_arrAbility[i]->Clone();
        assert(pAbil);
        m_arrAbility[i] = pAbil;
    }

    // Object Copy 복사
    for (UINT i = 0; i < (UINT)ObjectCopyType::END; ++i)
    {
        if (nullptr == _Origin.m_arrObject[i])
            continue;

        CKirbyObject* pObjCopy = _Origin.m_arrObject[i]->Clone();
        assert(pObjCopy);
        m_arrObject[i] = pObjCopy;
    }
}

CKirbyFSM::~CKirbyFSM()
{
    for (UINT i = 0; i < (UINT)AbilityCopyType::END; ++i)
    {
        if (m_arrAbility[i] != nullptr)
        {
            delete m_arrAbility[i];
            m_arrAbility[i] = nullptr;
        }
    }

    for (UINT i = 0; i < (UINT)ObjectCopyType::END; ++i)
    {
        if (m_arrObject[i] != nullptr)
        {
            delete m_arrObject[i];
            m_arrObject[i] = nullptr;
        }
    }

    if (m_StuffedCopyObj)
    {
        delete m_StuffedCopyObj;
    }
}

#include "CKirbyIdle.h"
#include "CKirbyRun.h"
#include "CKirbyRunStart.h"
#include "CKirbyJump.h"
#include "CKirbyJumpStart.h"
#include "CKirbyJumpFall.h"
#include "CKirbyLanding.h"
#include "CKirbyLandingEnd.h"
#include "CKirbyHovering.h"
#include "CKirbyHoveringStart.h"
#include "CKirbyHoveringFall.h"
#include "CKirbyHoveringLimit.h"
#include "CKirbyHoveringFallLimit.h"
#include "CKirbyHoveringLanding.h"
#include "CKirbyHoveringSpit.h"
#include "CKirbyAttack.h"
#include "CKirbyAttackEnd.h"
#include "CKirbyAttackCharge1.h"
#include "CKirbyAttackCharge1Start.h"
#include "CKirbyAttackCharge1End.h"
#include "CKirbyAttackCharge1Run.h"
#include "CKirbyAttackCharge2.h"
#include "CKirbyAttackCharge2Start.h"
#include "CKirbyAttackCharge2Run.h"
#include "CKirbyStuffed.h"
#include "CKirbyStuffedIdle.h"
#include "CKirbyStuffedRun.h"
#include "CKirbyStuffedJump.h"
#include "CKirbyStuffedJumpFall.h"
#include "CKirbyStuffedLanding.h"
#include "CKirbyStuffedEat.h"
#include "CKirbyGuard.h"
#include "CKirbySlideStart.h"
#include "CKirbySlide.h"
#include "CKirbySlideEnd.h"
#include "CKirbyDodgeStart.h"
#include "CKirbyDodge1.h"
#include "CKirbyDodge2.h"
#include "CKirbyDamage.h"
#include "CKirbyBackJump.h"
#include "CKirbyChangeAbility.h"
#include "CKirbyChangeAbilityEnd.h"
#include "CKirbyChangeAbilityWait.h"

void CKirbyFSM::begin()
{
    // State Init
    if (!GetOwner()->GetChildObject(L"Vacuum Collider") || !GetOwner()->GetChildObject(L"Vacuum Collider")->GetScript<CKirbyVacuumCollider>())
    {
        MessageBox(nullptr, L"Player에 자식 오브젝트 'Vacuum Collider'가 존재하지 않습니다", L"Player FSM begin() 실패", MB_OK);
        return;
    }

    m_VacuumCollider = GetOwner()->GetChildObject(L"Vacuum Collider")->GetScript<CKirbyVacuumCollider>();

    // State 추가
    AddState(L"IDLE", new CKirbyIdle);
    AddState(L"RUN", new CKirbyRun);
    AddState(L"RUN_START", new CKirbyRunStart);
    AddState(L"JUMP", new CKirbyJump);
    AddState(L"JUMP_START", new CKirbyJumpStart);
    AddState(L"JUMP_FALL", new CKirbyJumpFall);
    AddState(L"LANDING", new CKirbyLanding);
    AddState(L"LANDING_END", new CKirbyLandingEnd);
    AddState(L"HOVERING", new CKirbyHovering);
    AddState(L"HOVERING_START", new CKirbyHoveringStart);
    AddState(L"HOVERING_FALL", new CKirbyHoveringFall);
    AddState(L"HOVERING_LIMIT", new CKirbyHoveringLimit);
    AddState(L"HOVERING_FALL_LIMIT", new CKirbyHoveringFallLimit);
    AddState(L"HOVERING_LANDING", new CKirbyHoveringLanding);
    AddState(L"HOVERING_SPIT", new CKirbyHoveringSpit);
    AddState(L"ATTACK", new CKirbyAttack);
    AddState(L"ATTACK_END", new CKirbyAttackEnd);
    AddState(L"ATTACK_CHARGE1", new CKirbyAttackCharge1);
    AddState(L"ATTACK_CHARGE1_START", new CKirbyAttackCharge1Start);
    AddState(L"ATTACK_CHARGE1_END", new CKirbyAttackCharge1End);
    AddState(L"ATTACK_CHARGE1_RUN", new CKirbyAttackCharge1Run);
    AddState(L"ATTACK_CHARGE2", new CKirbyAttackCharge2);
    AddState(L"ATTACK_CHARGE2_START", new CKirbyAttackCharge2Start);
    AddState(L"ATTACK_CHARGE2_RUN", new CKirbyAttackCharge2Run);
    AddState(L"STUFFED", new CKirbyStuffed);
    AddState(L"STUFFED_IDLE", new CKirbyStuffedIdle);
    AddState(L"STUFFED_RUN", new CKirbyStuffedRun);
    AddState(L"STUFFED_JUMP", new CKirbyStuffedJump);
    AddState(L"STUFFED_JUMP_FALL", new CKirbyStuffedJumpFall);
    AddState(L"STUFFED_LANDING", new CKirbyStuffedLanding);
    AddState(L"STUFFED_EAT", new CKirbyStuffedEat);
    AddState(L"GUARD", new CKirbyGuard);
    AddState(L"SLIDE_START", new CKirbySlideStart);
    AddState(L"SLIDE", new CKirbySlide);
    AddState(L"SLIDE_END", new CKirbySlideEnd);
    AddState(L"DODGE_START", new CKirbyDodgeStart);
    AddState(L"DODGE1", new CKirbyDodge1);
    AddState(L"DODGE2", new CKirbyDodge2);
    AddState(L"DAMAGE", new CKirbyDamage);
    AddState(L"BACKJUMP", new CKirbyBackJump);
    AddState(L"CHANGE_ABILITY", new CKirbyChangeAbility);
    AddState(L"CHANGE_ABILITY_WAIT", new CKirbyChangeAbilityWait);
    AddState(L"CHANGE_ABILITY_END", new CKirbyChangeAbilityEnd);

    ChangeState(L"IDLE");
}

void CKirbyFSM::tick()
{
    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        m_ChargeAccTime += DT;
    }

    if (m_bHovering)
    {
        m_HoveringAccTime += DT;
    }

    // 무적상태 관리
    if (m_bInvincible)
    {
        m_InvincibleAcc += DT;

        m_EmissiveCoef += 3.f * DT;

        if (m_EmissiveCoef > 0.6f)
        {
            m_EmissiveCoef -= 0.6f;
        }

        if (m_EmissiveCoef > 0.3f)
        {
            PLAYERMTRL->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.6f - m_EmissiveCoef);
        }
        else
        {
            PLAYERMTRL->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_EmissiveCoef);
        }

        if (m_InvincibleAcc > m_InvincibleDuration)
        {
            m_bInvincible = false;
            m_InvincibleAcc = 0.f;
            m_EmissiveCoef = 0.f;
            PLAYERMTRL->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);
        }
    }

    CFSMScript::tick();
}

void CKirbyFSM::ChangeAbilityCopy(AbilityCopyType _Type)
{
    m_NextAbility = _Type;
    ChangeState(L"CHANGE_ABILITY");
    m_CurAbility = _Type;
}

void CKirbyFSM::ChangeObjectCopy(ObjectCopyType _Type)
{
    ChangeState(L"CHANGE_OBJECT");
    m_CurObject = _Type;
}

void CKirbyFSM::StartStuffed(CGameObject* _Target)
{
    m_StuffedCopyObj = _Target;
    m_bStuffed = true;
}

void CKirbyFSM::DrawingCollisionEnter(CGameObject* _CollisionObject)
{
    m_VacuumCollider->DrawingCollisionEnter(_CollisionObject);
}

void CKirbyFSM::SetHovering(bool _bHovering)
{
    if (m_bHovering != _bHovering)
    {
        ClearHoveringAccTime();
    }

    m_bHovering = _bHovering;
}

void CKirbyFSM::ClearStuff()
{
    m_bStuffed = false;

    if (m_StuffedCopyObj)
    {
        delete m_StuffedCopyObj;
    }
}

bool CKirbyFSM::IsDrawing() const
{
    return m_VacuumCollider->IsDrawing();
}

void CKirbyFSM::SaveToLevelFile(FILE* _File)
{
}

void CKirbyFSM::LoadFromLevelFile(FILE* _File)
{
}
