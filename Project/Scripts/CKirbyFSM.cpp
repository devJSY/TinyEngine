#include "pch.h"
#include "CKirbyFSM.h"
#include "CKirbyAbility.h"
#include "CKirbyObject.h"
#include "CKirbyVacuumCollider.h"

#include "CKirbyAbility_Normal.h"
#include "CKirbyAbility_Fire.h"
#include "CKirbyAbility_Cutter.h"
#include "CKirbyAbility_Sword.h"
#include "CKirbyAbility_Sleep.h"
#include "CKirbyObject_Cone.h"
#include "CKirbyObject_Lightbulb.h"
#include "CKirbyObject_VendingMachine.h"

CKirbyFSM::CKirbyFSM()
    : CFSMScript(KIRBYFSM)
    , m_arrAbility{}
    , m_arrObject{}
    , m_CurAbility(AbilityCopyType::NORMAL)
    , m_NextAbility(AbilityCopyType::NONE)
    , m_CurObject(ObjectCopyType::NONE)
    , m_CurHat(nullptr)
    , m_CurWeapon(nullptr)
    , m_StuffedObj(nullptr)
    , m_VacuumCollider(nullptr)
    , m_ComboLevel(0)
    , m_SlideComboLevel(0)
    , m_ComboAccTime(0.f)
    , m_ChargeAccTime(0.f)
    , m_HoveringAccTime(0.f)
    , m_HoveringLimitTime(7.f)
    , m_LastJump(LastJumpType::HIGH)
    , m_DodgeType(DodgeType::NONE)
    , m_bAttackEvent(false)
    , m_bStuffed(false)
    , m_bHovering(false)
    , m_bInvincible(false)
    , m_InvincibleAcc(0.f)
    , m_InvincibleDuration(0.f)
    , m_EmissiveCoef(0.f)
    , m_GlidingDuration(1.7f)
    , m_GlidingAcc(0.f)
    , m_YPressedTime(0.f)
    , m_DropCopyTime(1.f)
    , m_bDroppable(false)
    , m_LeftCanCount(0)
{
    // @TODO Copy Type마다 추가
    m_arrAbility[(UINT)AbilityCopyType::NORMAL] = new CKirbyAbility_Normal();
    m_arrAbility[(UINT)AbilityCopyType::FIRE] = new CKirbyAbility_Fire();
    m_arrAbility[(UINT)AbilityCopyType::CUTTER] = new CKirbyAbility_Cutter();
    m_arrAbility[(UINT)AbilityCopyType::SWORD] = new CKirbyAbility_Sword();
    m_arrAbility[(UINT)AbilityCopyType::SLEEP] = new CkirbyAbility_Sleep();

    m_arrObject[(UINT)ObjectCopyType::CONE] = new CKirbyObject_Cone();
    m_arrObject[(UINT)ObjectCopyType::VENDING_MACHINE] = new CKirbyObject_VendingMachine();
    m_arrObject[(UINT)ObjectCopyType::LIGHT] = new CKirbyObject_Lightbulb();
}

CKirbyFSM::CKirbyFSM(const CKirbyFSM& _Origin)
    : CFSMScript(_Origin)
    , m_arrAbility{}
    , m_arrObject{}
    , m_CurAbility(_Origin.m_CurAbility)
    , m_CurObject(_Origin.m_CurObject)
    , m_StuffedObj(nullptr)
    , m_VacuumCollider(nullptr)
    , m_ComboLevel(0)
    , m_ComboAccTime(0.f)
    , m_ChargeAccTime(0.f)
    , m_HoveringAccTime(0.f)
    , m_HoveringLimitTime(_Origin.m_HoveringLimitTime)
    , m_LastJump(LastJumpType::HIGH)
    , m_bAttackEvent(false)
    , m_bStuffed(false)
    , m_bHovering(false)
    , m_bInvincible(false)
    , m_InvincibleAcc(_Origin.m_InvincibleAcc)
    , m_InvincibleDuration(_Origin.m_InvincibleDuration)
    , m_EmissiveCoef(_Origin.m_EmissiveCoef)
    , m_GlidingDuration(1.7f)
    , m_GlidingAcc(0.f)
    , m_LeftCanCount(_Origin.m_LeftCanCount)
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
}

#include "CKirbyIdle.h"
#include "CKirbyIdleStart.h"
#include "CKirbyDeath.h"
#include "CKirbyDeathEnd.h"
#include "CKirbyRun.h"
#include "CKirbyRunStart.h"
#include "CKirbyRunEnd.h"
#include "CKirbyJump.h"
#include "CKirbyJumpStart.h"
#include "CKirbyJumpFall.h"
#include "CKirbyJumpAttack.h"
#include "CKirbyJumpAttackStart.h"
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
#include "CKirbyAttackFailed.h"
#include "CKirbyAttackFailedEnd.h"
#include "CKirbyAttackStart.h"
#include "CKirbyAttackEnd.h"
#include "CKirbyAttackCombo1.h"
#include "CKirbyAttackCombo1End.h"
#include "CKirbyAttackCombo2.h"
#include "CKirbyAttackCombo2End.h"
#include "CKirbyAttackCharge1.h"
#include "CKirbyAttackCharge1Start.h"
#include "CKirbyAttackCharge1End.h"
#include "CKirbyAttackCharge1Run.h"
#include "CKirbyAttackCharge2.h"
#include "CKirbyAttackCharge2Start.h"
#include "CKirbyAttackCharge2Run.h"
#include "CKirbyAttackCharge2Slash.h"
#include "CKirbyAttackCharge2SlashStart.h"
#include "CKirbyAttackCharge2SlashEnd.h"
#include "CKirbyAttackCharge3.h"
#include "CKirbyAttackCharge3Start.h"
#include "CKirbyAttackCharge3End.h"
#include "CKirbyAttackAirGuard.h"
#include "CKirbyAttackAirGuardEnd.h"
#include "CKirbyAttackAirGuardCharge.h"
#include "CKirbyAttackAirGuardChargeStart.h"
#include "CKirbyStuffed.h"
#include "CKirbyStuffedIdle.h"
#include "CKirbyStuffedRun.h"
#include "CKirbyStuffedJump.h"
#include "CKirbyStuffedJumpFall.h"
#include "CKirbyStuffedLanding.h"
#include "CKirbyStuffedEat.h"
#include "CKirbyGuard.h"
#include "CKirbyGuardRun.h"
#include "CKirbySlide.h"
#include "CKirbySlideStart.h"
#include "CKirbySlideEnd.h"
#include "CKirbySlideAttack.h"
#include "CKirbyDodgeStart.h"
#include "CKirbyDodge1.h"
#include "CKirbyDodge2.h"
#include "CKirbyDamage.h"
#include "CKirbyBackJump.h"
#include "CKirbyChangeAbility.h"
#include "CKirbyChangeAbilityEnd.h"
#include "CKirbyChangeAbilityWait.h"
#include "CKirbyDropAbility.h"
#include "CKirbyBurningPre.h"
#include "CKirbyBurningStart.h"
#include "CKirbyBurning.h"
#include "CKirbyBurningEnd.h"
#include "CKirbyFinalCutterRise.h"
#include "CKirbyFinalCutterDrop.h"
#include "CKirbyFinalCutterEnd.h"
#include "CKirbyFinalCutterEndAfter.h"
#include "CKirbyChangeObject.h"
#include "CKirbyChangeObjectEnd.h"
#include "CKirbyDropObjectStart.h"
#include "CKirbyDropObject.h"

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
    AddState(L"IDLE_START", new CKirbyIdleStart);
    AddState(L"DEATH", new CKirbyDeath);
    AddState(L"DEATH_END", new CKirbyDeathEnd);
    AddState(L"RUN", new CKirbyRun);
    AddState(L"RUN_START", new CKirbyRunStart);
    AddState(L"RUN_END", new CKirbyRunEnd);
    AddState(L"JUMP", new CKirbyJump);
    AddState(L"JUMP_START", new CKirbyJumpStart);
    AddState(L"JUMP_FALL", new CKirbyJumpFall);
    AddState(L"JUMP_ATTACK", new CKirbyJumpAttack);
    AddState(L"JUMP_ATTACK_START", new CKirbyJumpAttackStart);
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
    AddState(L"ATTACK_FAILED", new CKirbyAttackFailed);
    AddState(L"ATTACK_FAILEDEND", new CKirbyAttackFailedEnd);
    AddState(L"ATTACK_START", new CKirbyAttackStart);
    AddState(L"ATTACK_END", new CKirbyAttackEnd);
    AddState(L"ATTACK_COMBO1", new CKirbyAttackCombo1);
    AddState(L"ATTACK_COMBO1_END", new CKirbyAttackCombo1End);
    AddState(L"ATTACK_COMBO2", new CKirbyAttackCombo2);
    AddState(L"ATTACK_COMBO2_END", new CKirbyAttackCombo2End);
    AddState(L"ATTACK_CHARGE1", new CKirbyAttackCharge1);
    AddState(L"ATTACK_CHARGE1_START", new CKirbyAttackCharge1Start);
    AddState(L"ATTACK_CHARGE1_END", new CKirbyAttackCharge1End);
    AddState(L"ATTACK_CHARGE1_RUN", new CKirbyAttackCharge1Run);
    AddState(L"ATTACK_CHARGE2", new CKirbyAttackCharge2);
    AddState(L"ATTACK_CHARGE2_START", new CKirbyAttackCharge2Start);
    AddState(L"ATTACK_CHARGE2_RUN", new CKirbyAttackCharge2Run);
    AddState(L"ATTACK_CHARGE2_SLASH", new CKirbyAttackCharge2Slash);
    AddState(L"ATTACK_CHARGE2_SLASH_START", new CKirbyAttackCharge2SlashStart);
    AddState(L"ATTACK_CHARGE2_SLASH_END", new CKirbyAttackCharge2SlashEnd);
    AddState(L"ATTACK_CHARGE3", new CKirbyAttackCharge3);
    AddState(L"ATTACK_CHARGE3_START", new CKirbyAttackCharge3Start);
    AddState(L"ATTACK_CHARGE3_END", new CKirbyAttackCharge3End);
    AddState(L"ATTACK_AIRGUARD", new CKirbyAttackAirGuard);
    AddState(L"ATTACK_AIRGUARD_END", new CKirbyAttackAirGuardEnd);
    AddState(L"ATTACK_AIRGUARD_CHARGE", new CKirbyAttackAirGuardCharge);
    AddState(L"ATTACK_AIRGUARD_CHARGE_START", new CKirbyAttackAirGuardChargeStart);
    AddState(L"STUFFED", new CKirbyStuffed);
    AddState(L"STUFFED_IDLE", new CKirbyStuffedIdle);
    AddState(L"STUFFED_RUN", new CKirbyStuffedRun);
    AddState(L"STUFFED_JUMP", new CKirbyStuffedJump);
    AddState(L"STUFFED_JUMP_FALL", new CKirbyStuffedJumpFall);
    AddState(L"STUFFED_LANDING", new CKirbyStuffedLanding);
    AddState(L"STUFFED_EAT", new CKirbyStuffedEat);
    AddState(L"GUARD", new CKirbyGuard);
    AddState(L"GUARD_RUN", new CKirbyGuardRun);
    AddState(L"SLIDE_START", new CKirbySlideStart);
    AddState(L"SLIDE", new CKirbySlide);
    AddState(L"SLIDE_END", new CKirbySlideEnd);
    AddState(L"SLIDE_ATTACK", new CKirbySlideAttack);
    AddState(L"DODGE_START", new CKirbyDodgeStart);
    AddState(L"DODGE1", new CKirbyDodge1);
    AddState(L"DODGE2", new CKirbyDodge2);
    AddState(L"DAMAGE", new CKirbyDamage);
    AddState(L"BACKJUMP", new CKirbyBackJump);
    AddState(L"CHANGE_ABILITY", new CKirbyChangeAbility);
    AddState(L"CHANGE_ABILITY_WAIT", new CKirbyChangeAbilityWait);
    AddState(L"CHANGE_ABILITY_END", new CKirbyChangeAbilityEnd);
    AddState(L"DROP_ABILITY", new CKirbyDropAbility);

    // Fire
    AddState(L"BURNING_PRE", new CKirbyBurningPre);
    AddState(L"BURNING_START", new CKirbyBurningStart);
    AddState(L"BURNING", new CKirbyBurning);
    AddState(L"BURNING_END", new CKirbyBurningEnd);

    // Cutter
    AddState(L"FINALCUTTERRISE", new CKirbyFinalCutterRise);
    AddState(L"FINALCUTTERDROP", new CKirbyFinalCutterDrop);
    AddState(L"FINALCUTTEREND", new CKirbyFinalCutterEnd);
    AddState(L"FINALCUTTERENDAFTER", new CKirbyFinalCutterEndAfter);

    AddState(L"CHANGE_OBJECT", new CKirbyChangeObject);
    AddState(L"CHANGE_OBJECT_END", new CKirbyChangeObjectEnd);
    AddState(L"DROP_OBJECT", new CKirbyDropObject);
    AddState(L"DROP_OBJECT_START", new CKirbyDropObjectStart);

    ChangeState(L"IDLE");
}

void CKirbyFSM::tick()
{
    if (m_ComboLevel != 0)
    {
        m_ComboAccTime += DT;

        if (m_ComboAccTime >= GetCurAbility()->GetComboSuccessTime())
        {
            SetComboLevel(0);
        }
    }

    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        m_ChargeAccTime += DT;
    }

    if (m_bHovering)
    {
        m_HoveringAccTime += DT;
    }

    if (m_bDroppable && (KEY_TAP(KEY::Y) || KEY_PRESSED(KEY::Y)))
    {
        if (m_CurObject == ObjectCopyType::NONE && m_CurAbility == AbilityCopyType::NORMAL)
        {
        }
        else
        {
            m_YPressedTime += DT;
        }
    }
    else if (m_YPressedTime >= 0.f && KEY_RELEASED(KEY::Y))
    {
        m_YPressedTime = 0.f;
    }

    // 무적 상태 관리
    if (m_bInvincible && m_InvincibleDuration != -1.f)
    {
        m_InvincibleAcc += DT;

        if (m_InvincibleAcc > m_InvincibleDuration)
        {
            // 무적 상태 해제
            m_bInvincible = false;
        }
    }

    // Emissive 상태 관리
    if (m_bEmissive)
    {
        m_EmissiveAcc += DT;

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

        if (m_EmissiveAcc > m_EmissiveDuration)
        {
            // Emissive 상태 해제
            m_bEmissive = false;
            m_EmissiveCoef = 0.f;
            PLAYERMTRL->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);
        }
    }

    CFSMScript::tick();
}
void CKirbyFSM::ChangeAbilityCopy(AbilityCopyType _Type)
{
    // Drop Ability 요청
    if (_Type == AbilityCopyType::NORMAL)
    {
        m_CurAbility = _Type;
    }

    // Change Ability 요청
    else
    {
        if (_Type == AbilityCopyType::NONE)
        {
            MessageBox(nullptr, L"흡수할 Ability Type이 존재하지 않습니다", L"Change Ability 실패", MB_OK);
            return;
        }
        if (m_CurAbility != AbilityCopyType::NORMAL)
        {
            MessageBox(nullptr, L"Ability가 이미 존재합니다", L"Change Ability 실패", MB_OK);
            return;
        }

        m_NextAbility = _Type;
        ChangeState(L"CHANGE_ABILITY");
        m_CurAbility = _Type;
    }
}

void CKirbyFSM::ChangeObjectCopy(ObjectCopyType _Type)
{
    // Drop Object 요청
    if (_Type == ObjectCopyType::NONE)
    {
        m_CurObject = _Type;
    }

    // Change Object 요청
    else
    {
        if (m_CurObject != ObjectCopyType::NONE)
        {
            MessageBox(nullptr, L"Object가 이미 존재합니다", L"Change Object 실패", MB_OK);
            return;
        }

        // m_NextObject = _Type;
        // ChangeState(L"CHANGE_OBJECT");
        // m_CurObject = _Type;

        m_CurObject = _Type;
        ChangeState(L"CHANGE_OBJECT");
    }
}

void CKirbyFSM::StartStuffed(CGameObject* _Target)
{
    ClearStuff();

    m_StuffedObj = _Target;
    m_bStuffed = true;
}

void CKirbyFSM::DrawingCollisionEnter(CGameObject* _CollisionObject)
{
    m_VacuumCollider->DrawingCollisionEnter(_CollisionObject);
}

void CKirbyFSM::SetCurHat(CGameObject* _Hat)
{
    if (!_Hat)
        return;

    if (m_CurHat)
    {
        GamePlayStatic::DestroyGameObject(m_CurHat);
    }

    m_CurHat = _Hat;
}

void CKirbyFSM::SetCurWeapon(CGameObject* _Weapon)
{
    if (!_Weapon)
        return;

    if (m_CurWeapon)
    {
        GamePlayStatic::DestroyGameObject(m_CurWeapon);
    }

    m_CurWeapon = _Weapon;
}

void CKirbyFSM::SetHovering(bool _bHovering)
{
    if (m_bHovering != _bHovering)
    {
        ClearHoveringAccTime();
    }

    m_bHovering = _bHovering;
}

void CKirbyFSM::SetInvincible(bool _Invincible, float _Duration)
{
    if (_Duration == -1.f)
    {
        m_bInvincible = _Invincible;
        m_InvincibleDuration = _Duration;
        m_InvincibleAcc = 0.f;
    }
    else
    {
        if (_Invincible == true)
        {
            float RemainTime = m_InvincibleDuration - m_InvincibleAcc;

            if (RemainTime < _Duration)
            {
                m_bInvincible = _Invincible;
                m_InvincibleDuration = _Duration;
                m_InvincibleAcc = 0.f;
            }
        }
    }
}

void CKirbyFSM::SetEmissive(bool _Emissive, float _Duration)
{
    if (_Emissive == false)
    {
        m_bEmissive = false;
        m_EmissiveCoef = 0.f;
        PLAYERMTRL->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);

        m_EmissiveAcc = 0.f;
        m_EmissiveDuration = 0.f;
    }
    else
    {
        // 현재 적용되고 있는 Emissive가 있는 경우
        if (m_bEmissive)
        {
            float RemainTime = m_EmissiveDuration - m_EmissiveAcc;

            // 남은 Emissive 시간보다 더 긴 경우 새로 입력받은 Emissive를 적용
            if (RemainTime < _Duration)
            {
                m_EmissiveDuration = _Duration;
                m_EmissiveAcc = 0.f;
            }
        }
        // 현재 적용되고 있는 Emissive가 없는 경우 바로 적용한다.
        else
        {
            m_bEmissive = true;

            m_EmissiveDuration = _Duration;
            m_EmissiveAcc = 0.f;
        }
    }
}

void CKirbyFSM::ClearCurHatWeapon()
{
    if (m_CurHat)
    {
        GamePlayStatic::DestroyGameObject(m_CurHat);
        m_CurHat = nullptr;
    }

    if (m_CurWeapon)
    {
        GamePlayStatic::DestroyGameObject(m_CurWeapon);
        m_CurWeapon = nullptr;
    }
}

void CKirbyFSM::ClearStuff()
{
    m_bStuffed = false;

    if (m_StuffedObj)
    {
        GamePlayStatic::DestroyGameObject(m_StuffedObj);
        m_StuffedObj = nullptr;
    }
}

void CKirbyFSM::SubCanCount()
{
    --m_LeftCanCount;
    if (m_LeftCanCount < 0)
        m_LeftCanCount = 0;
}
bool CKirbyFSM::IsDrawing() const
{
    return m_VacuumCollider->IsDrawing();
}

UINT CKirbyFSM::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CFSMScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CKirbyFSM::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CFSMScript::SaveToLevelFile(_File);

    return MemoryByte;
}
