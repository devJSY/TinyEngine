#include "pch.h"
#include "CKirbyUnitScript.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CState.h"
#include "CKirbyCopyAbilityScript.h"
#include "CMomentaryObjScript.h"

#include <Engine/CTimeMgr.h>
#include "CLevelFlowMgr.h"

#include <Engine/CSphereCollider.h>
#include <Engine/CBoxCollider.h>

#include "CCameraController.h"

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
        _Origin.m_InitInfo.HP, // HP
        100.f,                 // MaxHP
        10.f,                  // Speed
        10.f,                  // Rotation Speed
        11.f,                  // JumpPower
        0.f,                   // ATK
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

    if (m_CurInfo.HP <= 0.f)
    {
        m_CurInfo.HP = 100.f;
    }

    m_AbilityBubble = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbyBubble.pref", L"prefab\\KirbyBubble.pref");
}

void CKirbyUnitScript::tick()
{
    ClearHitDir();
    m_PrevInfo = m_CurInfo;

    // update damage
    float NewDamage = DamageProc();
    wstring State = PLAYERFSM->GetCurState()->GetName();
    AbilityCopyType CurAbility = PLAYERFSM->GetCurAbilityIdx();
    ObjectCopyType CurObject = PLAYERFSM->GetCurObjectIdx();

    if (NewDamage > 0.f)
    {
        if (State == L"DODGE_START" || State == L"DODGE1" || State == L"DODGE2")
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
        else if (State == L"GUARD")
        {
            m_CurInfo.HP -= NewDamage * 0.25f;
        }
        else if (State == L"FALL" || PLAYERFSM->IsDrawing() ||
                 (CurObject == ObjectCopyType::NONE && CurAbility == AbilityCopyType::SLEEP && State.find(L"ATTACK") != wstring::npos))
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
            m_CurInfo.HP -= NewDamage;
            SetHitDirHorizen();
            PLAYERFSM->ChangeState(L"DAMAGE");

            if (NewDamage >= 8.f)
            {
                if (PLAYERFSM->GetCurObjectIdx() == ObjectCopyType::NONE && PLAYERFSM->GetCurAbilityIdx() != AbilityCopyType::NORMAL)
                {
                    DropAbility();
                }
            }

            NewDamage -= 5.f;

            if (NewDamage >= 0.f)
            {
                float Alpha = clamp((NewDamage / 5.f), 0.f, 1.f);

                float CameraIntencity = Lerp(30.f, 40.f, Alpha);
                float CameraDuration = Lerp(0.3f, 0.5f, Alpha);

                CAMERACTRL->Shake(CameraDuration, 60.f, CameraIntencity);
            }
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
    // object
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
    }

    // ability
    else
    {
        if (PLAYERFSM->GetCurAbilityIdx() == AbilityCopyType::SWORD)
        {
            m_CurInfo.HP += 1.5f;

            if (m_CurInfo.HP > m_CurInfo.MAXHP)
            {
                m_CurInfo.HP = m_CurInfo.MAXHP;
            }
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

void CKirbyUnitScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    // 흡입을 시작한 상태(타겟을 결정해 빨아들이기 시작한 상태)에서 충돌한 경우
    if (PLAYERFSM->IsDrawing())
    {
        PLAYERFSM->DrawingCollisionEnter(_OtherCollider->GetOwner());
        return;
    }
}

void CKirbyUnitScript::OnTriggerStay(CCollider* _OtherCollider)
{
    // 흡입을 시작한 상태(타겟을 결정해 빨아들이기 시작한 상태)에서 충돌한 경우
    if (PLAYERFSM->IsDrawing())
    {
        PLAYERFSM->DrawingCollisionEnter(_OtherCollider->GetOwner());
        return;
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
        pBubble->Transform()->SetLocalPos(InitPos);
        pBubble->Transform()->SetLocalRotation(InitRot);

        CKirbyCopyAbilityScript* pAbility = (CKirbyCopyAbilityScript*)CScriptMgr::GetScript(KIRBYCOPYABILITYSCRIPT);
        pAbility->SetAbilityType(PLAYERFSM->GetCurAbilityIdx());
        pBubble->AddComponent(pAbility);

        CGameObject* pMesh = new CGameObject;
        pMesh->SetName(L"Mesh");
        pMesh->AddComponent(new CTransform);
        pMesh->AddComponent(PLAYERFSM->GetCurHat()->MeshRender()->Clone());
        if (PLAYERFSM->GetCurHat()->Animator())
        {
            pMesh->AddComponent(PLAYERFSM->GetCurHat()->Animator()->Clone());
            pMesh->Animator()->Play(ANIMPREFIX("Deform"));
        }
        else
        {
            pMesh->Transform()->SetLocalRotation(Vec3(XMConvertToRadians(-90.f), XMConvertToRadians(180.f), 0.f));
        }

        pMesh->Transform()->SetAbsolute(false);
        pMesh->Transform()->SetLocalPos(Vec3(0.f, -0.4f, 0.f));
        pMesh->Transform()->SetLocalScale(Vec3(0.9f));
        pMesh->MeshRender()->SetCastShadow(false);

        GamePlayStatic::AddChildObject(pBubble, pMesh);

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

    MemoryByte += CUnitScript::SaveToLevelFile(_File);
    fwrite(&m_CurInfo.HP, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CKirbyUnitScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CUnitScript::LoadFromLevelFile(_File);
    fread(&m_InitInfo.HP, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}
