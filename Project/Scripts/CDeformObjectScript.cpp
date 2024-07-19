#include "pch.h"
#include "CDeformObjectScript.h"

CDeformObjectScript::CDeformObjectScript()
    : CScript(DEFORMOBJECTSCRIPT)
    , m_CurState(DEFORMOBJ_STATE::IDLE)
    , m_PrevState(DEFORMOBJ_STATE::IDLE)
    , rayDist(1.f)
{
}

CDeformObjectScript::CDeformObjectScript(const CDeformObjectScript& _Origin)
    : CScript(_Origin)
{
}

CDeformObjectScript::~CDeformObjectScript()
{
}

void CDeformObjectScript::begin()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("Wait"));

    if (GetOwner()->SphereCollider())
    {
        rayCenter = GetOwner()->SphereCollider()->GetCenter();
        rayDist = GetOwner()->SphereCollider()->GetRadius();
    }
    else if (GetOwner()->CapsuleCollider())
    {
        rayCenter = GetOwner()->CapsuleCollider()->GetCenter();
        rayDist = GetOwner()->CapsuleCollider()->GetHeight() / 2.f;
    }
    else if (GetOwner()->BoxCollider())
    {
        rayCenter = GetOwner()->BoxCollider()->GetCenter();
        rayDist = GetOwner()->BoxCollider()->GetSize().y / 2.f;
    }
}

void CDeformObjectScript::tick()
{
    static vector<wstring> vecCollision{L"World Static", L"World Dynamic"};
    RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Transform()->GetWorldPos() + rayCenter, Vec3(0.f, -1.f, 0.f), rayDist, vecCollision);
    
    if (m_CurState == DEFORMOBJ_STATE::IDLE)
    {
        // enter
        if (m_CurState != m_PrevState)
        {
            GetOwner()->Animator()->Play(ANIMPREFIX("Wait"));
        }

        // change state
        if (!Hit.pCollisionObj)
        {
            m_CurState = DEFORMOBJ_STATE::AIR;
        }
    }

    else if (m_CurState == DEFORMOBJ_STATE::AIR)
    {
        // enter
        if (m_CurState != m_PrevState)
        {
            GetOwner()->Animator()->Play(ANIMPREFIX("Fall"));
        }
        
        // change state
        if (Hit.pCollisionObj)
        {
            m_CurState = DEFORMOBJ_STATE::LANDING;
        }
    }

    else if (m_CurState == DEFORMOBJ_STATE::LANDING)
    {
        // enter
        if (m_CurState != m_PrevState)
        {
            GetOwner()->Animator()->Play(ANIMPREFIX("Landing"), false);
        }

        // change state
        if (GetOwner()->Animator()->IsFinish())
        {
            m_CurState = DEFORMOBJ_STATE::IDLE;
        }
    }
}

void CDeformObjectScript::SaveToLevelFile(FILE* _File)
{
}

void CDeformObjectScript::LoadFromLevelFile(FILE* _File)
{
}