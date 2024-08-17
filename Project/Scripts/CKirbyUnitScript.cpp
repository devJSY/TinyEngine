#include "pch.h"
#include "CKirbyUnitScript.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CState.h"
#include "CKirbyCopyAbilityScript.h"
#include "CMomentaryObjScript.h"

#include <Engine/CTimeMgr.h>

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

    m_AbilityBubble = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbyBubble.pref", L"prefab\\KirbyBubble.pref");
}

void CKirbyUnitScript::tick()
{
    ClearHitDir();
    m_PrevInfo = m_CurInfo;

    // update damage
    float NewDamage = DamageProc();
    wstring State = PLAYERFSM->GetCurState()->GetName();

    if (NewDamage > 0.f)
    {
        if (State == L"DODGE_START" || State == L"DODGE1" || State == L"DODGE2")
        {
            CTimeMgr::GetInst()->SetTimeScale(1.f, 0.5f);
        }
        else if (PLAYERFSM->IsInvincible())
        {
            m_HitHistory.clear();
        }
        else if (State == L"GUARD")
        {
            m_CurInfo.HP -= NewDamage * 0.25f;
        }
        else if (State == L"FALL" || PLAYERFSM->IsDrawing())
        {
            m_CurInfo.HP -= NewDamage;
        }
        else if (State.find(L"STUFFED") != wstring::npos)
        {
            m_CurInfo.HP -= NewDamage;
            SetHitDirHorizen();
            PLAYERFSM->ChangeState(L"STUFFED_DAMAGE");
        }
        else
        {
            if (PLAYERFSM->GetCurObjectIdx() == ObjectCopyType::NONE && PLAYERFSM->GetCurAbilityIdx() != AbilityCopyType::NORMAL)
            {
                DropAbility();
            }
            
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
        // 질량이 100 이상인 물체는 밀지 않는다.
        if (Hit.Collider->GetOwner()->Rigidbody() && Hit.Collider->GetOwner()->Rigidbody()->GetMass() > 100.f)
        {
            return;
        }

        Vec3 Force = Hit.Collider->Transform()->GetWorldPos() - PLAYER->Transform()->GetWorldPos();
        Force.y = 0.f;
        Force.Normalize();
        Hit.Collider->Rigidbody()->AddForce(Force * 10.f, ForceMode::Acceleration);
    }
}

void CKirbyUnitScript::DropAbility()
{
    // spawn ability bubble
    if (nullptr != m_AbilityBubble && PLAYERFSM->GetCurAbilityIdx() != AbilityCopyType::SLEEP)
    {
        CGameObject* pBubble = m_AbilityBubble->Instantiate();
        Vec3 InitPos = PLAYER->Transform()->GetWorldPos();
        Vec3 InitRot = PLAYER->Transform()->GetLocalRotation();
        float ScaleFactor = PLAYER->Transform()->GetLocalScale().x;
        InitPos += PLAYER->Transform()->GetLocalDir(DIR_TYPE::UP) * ScaleFactor;
        InitPos -= PLAYER->Transform()->GetLocalDir(DIR_TYPE::FRONT) * ScaleFactor;
        InitRot.x += XMConvertToRadians(-90.f);
        InitRot.y += XMConvertToRadians(180.f);

        CKirbyCopyAbilityScript* pAbility = (CKirbyCopyAbilityScript*)CScriptMgr::GetScript(KIRBYCOPYABILITYSCRIPT);
        pBubble->AddComponent(PLAYERFSM->GetCurHat()->MeshRender()->Clone());
        pBubble->AddComponent(pAbility);
        pBubble->Transform()->SetLocalPos(InitPos);
        pBubble->Transform()->SetLocalScale(Vec3(ScaleFactor * 0.8f));
        pBubble->Transform()->SetLocalRotation(InitRot);
        pBubble->MeshRender()->SetCastShadow(false);
        pAbility->SetAbilityType(PLAYERFSM->GetCurAbilityIdx());

        CMomentaryObjScript* pMomentaryObj = pBubble->GetScript<CMomentaryObjScript>();
        if (nullptr != pMomentaryObj)
        {
            pMomentaryObj->SetInitVelocity(Vec3(0.f, 10.f, 0.f));
            pMomentaryObj->SetPlayTime(10.f);
        }

        GamePlayStatic::SpawnGameObject(pBubble, LAYER_DYNAMIC);
    }

    // clear current ability
    PLAYERFSM->ClearYPressedTime();
    PLAYERFSM->ClearCurHatWeapon();
    PLAYERFSM->ChangeAbilityCopy(AbilityCopyType::NORMAL);
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
