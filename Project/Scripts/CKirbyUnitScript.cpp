#include "pch.h"
#include "CKirbyUnitScript.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CState.h"

#include <Engine/CTimeMgr.h>
#include "CLevelFlowMgr.h"

#include <Engine/CSphereCollider.h>
#include <Engine/CBoxCollider.h>

CKirbyUnitScript::CKirbyUnitScript()
    : CUnitScript(KIRBYUNITSCRIPT)
{
    UnitInfo KirbyInfo = {
        100.f, // HP
        100.f, // MaxHP
        10.f,  // Speed
        10.f,  // Rotation Speed
        11.f,  // JumpPower
        0.f,   // ATK
    };
    SetInitInfo(KirbyInfo);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "[Current] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "[Current] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "[Current] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "[Current] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "[Current] Jump Power");
}

CKirbyUnitScript::CKirbyUnitScript(const CKirbyUnitScript& _Origin)
    : CUnitScript(_Origin)
{
    UnitInfo KirbyInfo = {
        100.f, // HP
        100.f, // MaxHP
        10.f,  // Speed
        10.f,  // Rotation Speed
        11.f,  // JumpPower
        0.f,   // ATK
    };
    SetInitInfo(KirbyInfo);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "[Current] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "[Current] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "[Current] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "[Current] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "[Current] Jump Power");
}

CKirbyUnitScript::~CKirbyUnitScript()
{
}

void CKirbyUnitScript::begin()
{
    // Level에 진입시 InitInfo를 현재의 정보로 저장
    SetInfo(GetInitInfo());
}

void CKirbyUnitScript::tick()
{
    ClearHitDir();
    m_PrevInfo = m_CurInfo;

    // update damage
    float NewDamage = DamageProc();
    if (NewDamage > 0.f)
    {
        if (PLAYERFSM->GetCurState()->GetName() == L"DODGE_START" || PLAYERFSM->GetCurState()->GetName() == L"DODGE1" ||
            PLAYERFSM->GetCurState()->GetName() == L"DODGE2")
        {
            CTimeMgr::GetInst()->SetTimeScale(1.f, 0.5f);
            
            CGameObject* ManagerObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");

            if (ManagerObj)
            {
                CLevelFlowMgr* FlowMgr = ManagerObj->GetScript<CLevelFlowMgr>();
                if (FlowMgr)
                {
                    FlowMgr->OnRadialBlurEffect(1.f);
                }
            }

            
        }
        else if (PLAYERFSM->IsInvincible())
        {
            m_HitHistory.clear();
        }
        else if (PLAYERFSM->GetCurState()->GetName() == L"GUARD")
        {
            m_CurInfo.HP -= NewDamage * 0.25f;
        }
        else if (PLAYERFSM->GetCurState()->GetName() == L"FALL")
        {
            m_CurInfo.HP -= NewDamage;
        }
        else
        {
            m_CurInfo.HP -= NewDamage;
            SetHitDirHorizen();
            PLAYERFSM->ChangeState(L"DAMAGE");
        }
    }

    // HP 처리
    if (m_CurInfo.HP <= 0.f)
    {
        m_CurInfo.HP = 0.f;

        // FALL State의 경우 Death로 넘어가지 않고 직접 처리
        if (PLAYERFSM->GetCurState()->GetName() != L"FALL")
        {
            PLAYERFSM->ChangeState(L"DEATH");
        }
    }

    if (m_CurInfo.HP > m_CurInfo.MAXHP)
    {
        m_CurInfo.HP = m_CurInfo.MAXHP;
    }
}

void CKirbyUnitScript::AttackReward()
{
    if (PLAYERFSM->GetCurAbilityIdx() == AbilityCopyType::SWORD)
    {
        m_CurInfo.HP += 2.5f;

        if (m_CurInfo.HP > m_CurInfo.MAXHP)
        {
            m_CurInfo.HP = m_CurInfo.MAXHP;
        }
    }
}

void CKirbyUnitScript::BuffHP(float _HP)
{
    m_CurInfo.HP += _HP;

    if (m_CurInfo.HP > m_CurInfo.MAXHP)
    {
        m_CurInfo.HP = m_CurInfo.MAXHP;
    }
}


void CKirbyUnitScript::OnControllerColliderHit(ControllerColliderHit Hit)
{
    // Dynamic Layer인 경우: 상대 오브젝트에게 힘 가함
    if (Hit.Collider->GetOwner()->GetLayerIdx() == LAYER_DYNAMIC && Hit.Collider->Rigidbody())
    {
        Vec3 Force = Hit.Collider->Transform()->GetWorldPos() - PLAYER->Transform()->GetWorldPos();
        Force.y = 0.f;     
        Force.Normalize(); 

        Vec3 ForceApplied = Force * 10.f;

        if (Hit.Collider->GetOwner()->GetComponent(COMPONENT_TYPE::BOXCOLLIDER) != nullptr)
        {
            ForceApplied = Vector3::Zero;
        }

        Hit.Collider->Rigidbody()->AddForce(ForceApplied, ForceMode::Acceleration);
    }
}

UINT CKirbyUnitScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CKirbyUnitScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}
