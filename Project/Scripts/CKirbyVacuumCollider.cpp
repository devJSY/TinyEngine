#include "pch.h"
#include "CKirbyVacuumCollider.h"
#include "CPlayerMgr.h"
#include "CKirbyMoveController.h"
#include "CMonsterUnitScript.h"
#include "CKirbyCopyAbilityScript.h"
#include "CKirbyCopyObjScript.h"
#include "CKirbyCantEat.h"
#include <Engine\CCollider.h>

CKirbyVacuumCollider::CKirbyVacuumCollider()
    : CScript(KIRBYVACUUMCOLLIDER)
    , m_FindTarget(nullptr)
    , m_FindType(EatType::NONE)
    , m_FindAbilityType(AbilityCopyType::NONE)
    , m_FindObjType(ObjectCopyType::NONE)
    , m_FindDistance(FBXSDK_FLOAT_MAX)
    , m_FindHoldAccTime(0.f)
    , m_FindHoldTime(0.f)
    , m_DrawingAccTime(0.f)
    , m_DrawingTime(0.5f)
    , m_bDrawing(false)
{
}

CKirbyVacuumCollider::CKirbyVacuumCollider(const CKirbyVacuumCollider& _Origin)
    : CScript(KIRBYVACUUMCOLLIDER)
    , m_FindTarget(nullptr)
    , m_FindType(EatType::NONE)
    , m_FindAbilityType(AbilityCopyType::NONE)
    , m_FindObjType(ObjectCopyType::NONE)
    , m_FindDistance(FBXSDK_FLOAT_MAX)
    , m_FindHoldAccTime(0.f)
    , m_FindHoldTime(0.f)
    , m_DrawingAccTime(0.f)
    , m_DrawingTime(_Origin.m_DrawingTime)
    , m_bDrawing(false)
{
}

CKirbyVacuumCollider::~CKirbyVacuumCollider()
{
}

void CKirbyVacuumCollider::begin()
{
    if (!GetOwner()->SphereCollider())
    {
        MessageBox(nullptr, L"Vacuum Collider에 Collider 컴포넌트가 존재하지 않습니다 (직접 생성 요망)", L"CKirbyVacuumCollider 경고", MB_OK);

        CSphereCollider* newCol = new CSphereCollider();
        newCol->SetTrigger(true);
        GetOwner()->AddComponent(newCol);
    }

    GetOwner()->SphereCollider()->SetEnabled(false);
}

void CKirbyVacuumCollider::tick()
{
    // Drawing 상태 결정
    CheckDrawing();

    // Drawing 상태라면 타겟 오브젝트 빨아들임
    DrawingTarget();
}

void CKirbyVacuumCollider::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (m_bDrawing || m_FindTarget)
        return;

    // 흡수할 수 있는 물체라면
    int i = _OtherCollider->GetOwner()->GetLayerIdx();
    if (_OtherCollider->GetOwner()->GetLayerIdx() != LAYER_DYNAMIC && _OtherCollider->GetOwner()->GetLayerIdx() != LAYER_MONSTER)
        return;

    bool bChanged = false;
    AbilityCopyType newAbility = AbilityCopyType::NONE;
    ObjectCopyType newObject = ObjectCopyType::NONE;
    float newHoldTime = 0.f;
    float newDist = (PLAYER->Transform()->GetWorldPos() - _OtherCollider->Transform()->GetWorldPos()).Length();
    EatType newType = GetEatType(_OtherCollider->GetOwner(), newAbility, newObject, newHoldTime);

    if (newType == EatType::NONE)
        return;

    // 현재 흡수예정인 오브젝트와 비교해 우선순위 판단
    if (PLAYERFSM->GetCurAbilityIdx() == AbilityCopyType::NORMAL)
    {
        if ((UINT)newType < (UINT)m_FindType)
        {
            bChanged = true;
        }
        else if ((UINT)newType == (UINT)m_FindType)
        {
            if (newDist < m_FindDistance)
            {
                bChanged = true;
            }
        }
    }
    else
    {
        if (newType == EatType::Copy_Object && newDist < m_FindDistance)
        {
            bChanged = true;
        }
    }

    if (bChanged)
    {
        PLAYERCTRL->LockJump();
        PLAYERCTRL->LockDirection();
        PLAYERCTRL->LockMove();
        PLAYERFSM->SetGlobalState(true);

        m_FindTarget = _OtherCollider->GetOwner();
        m_FindType = newType;
        m_FindDistance = newDist;
        m_FindAbilityType = newAbility;
        m_FindObjType = newObject;
        m_FindHoldTime = newHoldTime;
    }
}

void CKirbyVacuumCollider::DrawingCollisionEnter(CGameObject* _CollisionObject)
{
    if (!m_bDrawing || _CollisionObject != m_FindTarget)
        return;

    m_bDrawing = false;
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockMove();
    PLAYERFSM->SetGlobalState(false);

    // Change Player State
    switch (m_FindType)
    {
    case EatType::Copy_Ability:
    case EatType::Copy_Monster:
        PLAYERFSM->ChangeAbilityCopy(m_FindAbilityType);
        GamePlayStatic::DestroyGameObject(m_FindTarget);
        break;
    case EatType::Copy_Object:
        PLAYERFSM->ChangeObjectCopy(m_FindObjType);
        GamePlayStatic::DestroyGameObject(m_FindTarget);
        break;
    case EatType::Etc:
    case EatType::Monster:
        PLAYERFSM->StartStuffed(m_FindTarget);
        m_FindTarget->SetActive(false);
        break;
    }

    m_FindTarget = nullptr;
}

void CKirbyVacuumCollider::EnableCollider(bool _bEnable)
{
    GetOwner()->SphereCollider()->SetEnabled(_bEnable);

    if (_bEnable)
    {
        m_FindTarget = nullptr;
        m_FindType = EatType::NONE;
        m_FindAbilityType = AbilityCopyType::NONE;
        m_FindObjType = ObjectCopyType::NONE;
        m_bDrawing = false;
        m_FindDistance = FBXSDK_FLOAT_MAX;
        m_FindHoldTime = 0.f;
    }
}

EatType CKirbyVacuumCollider::GetEatType(CGameObject* _pObj, AbilityCopyType& _outAbility, ObjectCopyType& _outObj, float& _outHoldTime)
{
    if (_pObj->GetLayerIdx() == LAYER_DYNAMIC)
    {
        if (_pObj->GetScript<CKirbyCantEat>())
        {
            return EatType::NONE;
        }
        else if (_pObj->GetScript<CKirbyCopyObjScript>())
        {
            _outObj = _pObj->GetScript<CKirbyCopyObjScript>()->GetObjType();
            return EatType::Copy_Object;
        }
        else if (_pObj->GetScript<CKirbyCopyAbilityScript>())
        {
            _outAbility = _pObj->GetScript<CKirbyCopyAbilityScript>()->GetAbilityType();
            return EatType::Copy_Ability;
        }
        else
        {
            return EatType::Etc;
        }
    }

    else if (_pObj->GetLayerIdx() == LAYER_MONSTER)
    {
        if (_pObj->GetScript<CKirbyCantEat>())
        {
            return EatType::NONE;
        }
        else if (_pObj->GetScript<CKirbyCopyAbilityScript>())
        {
            _outAbility = _pObj->GetScript<CKirbyCopyAbilityScript>()->GetAbilityType();
            return EatType::Copy_Monster;
        }
        else
        {
            CMonsterUnitScript* pMonster = _pObj->GetScript<CMonsterUnitScript>();

            if (!pMonster || !pMonster->IsEatable())
            {
                return EatType::NONE;
            }

            _outHoldTime = pMonster->GetResistTime();
            return EatType::Monster;
        }
    }

    return EatType::NONE;
}

void CKirbyVacuumCollider::CheckDrawing()
{
    // 예외처리 : 이미 Drawing이거나, Drawing Target이 없는 경우
    if (m_bDrawing || !m_FindTarget)
        return;

    // target object가 monster라면 버티는 상태로 변경시켜줌
    if (m_FindType == EatType::Copy_Monster || m_FindType == EatType::Monster)
    {
        m_FindTarget->Rigidbody()->SetVelocity(Vec3());
        m_FindTarget->Rigidbody()->SetAngularVelocity(Vec3());

        m_FindTarget->GetScript<CMonsterUnitScript>()->SetResistState(true);
    }

    // clear vacuum collider
    EnableCollider(false);

    // clone
    CGameObject* TargetClone = m_FindTarget->Clone();
    int Layer = m_FindTarget->GetLayerIdx();
    wstring Name = L"(Vacuum Target) " + m_FindTarget->GetName();
    GamePlayStatic::DestroyGameObject(m_FindTarget);

    m_FindTarget = TargetClone;
    m_FindTarget->SetName(Name);
    GamePlayStatic::SpawnGameObject(m_FindTarget, Layer);

    // clean-up script & copmonents
    m_FindTarget->RemoveComponent(COMPONENT_TYPE::RIGIDBODY);

    std::deque<CGameObject*> Queue{m_FindTarget};
    while (!Queue.empty())
    {
        CGameObject* Front = Queue.front();
        Queue.pop_front();

        for (CGameObject* Child : Front->GetChildObject())
        {
            Queue.push_back(Child);
        }

        // remove script
        for (CScript* Script : Front->GetScripts())
        {
            if (Script->GetScriptType() != KIRBYBULLETSIZE)
            {
                Front->RemoveScript(Script);
            }
        }

        // change collider
        CCollider* Collider = Front->GetComponent<CCollider>();

        if (Collider)
        {
            Collider->SetTrigger(true);
        }
    }

    //@Effect 반짝 효과 셰이더
    m_bDrawing = true;
    m_FindHoldAccTime = 0.f;
    m_DrawingAccTime = 0.f;
}

void CKirbyVacuumCollider::DrawingTarget()
{
    if (!m_bDrawing || !m_FindTarget)
        return;

    // safety check
    Vec3 Dist = PLAYER->Transform()->GetWorldPos() - m_FindTarget->Transform()->GetWorldPos();
    float Radius = PLAYER->GetChildObject(L"Body Collider")->CapsuleCollider()->GetRadius();
    Radius *= PLAYER->Transform()->GetWorldScale().x;

    if (Dist.Length() <= Radius + 1e-3)
    {
        DrawingCollisionEnter(m_FindTarget);
        return;
    }

    // move target pos
    Vec3 Dir = (m_FindTarget->Transform()->GetWorldPos() - PLAYER->Transform()->GetWorldPos()).Normalize();
    Vec3 NewPos = PLAYER->Transform()->GetWorldPos();

    if (m_FindHoldAccTime < m_FindHoldTime)
    {
        m_FindHoldAccTime += DT;

        float t = m_FindHoldAccTime / m_FindHoldTime;
        NewPos += Dir * (m_FindDistance - 10.f * t);
    }
    else
    {
        m_DrawingAccTime += DT;

        float t = clamp(m_DrawingAccTime / m_DrawingTime, 0.f, 1.f);
        NewPos = PLAYER->Transform()->GetWorldPos();

        if (m_FindHoldTime > 0.f)
        {
            NewPos += Dir * ((m_FindDistance - 10.f) * cosf(t * XM_PI / 2.f));
        }
        else
        {
            NewPos += Dir * m_FindDistance * cosf(t * XM_PI / 2.f);
        }
    }

    m_FindTarget->Transform()->SetWorldPos(NewPos);
}

UINT CKirbyVacuumCollider::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CKirbyVacuumCollider::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}