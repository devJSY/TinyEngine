#include "pch.h"
#include "CAnimationTestScript.h"

CAnimationTestScript::CAnimationTestScript()
    : CScript(ANIMATIONTESTSCRIPT)
{
}

CAnimationTestScript::~CAnimationTestScript()
{
}

void CAnimationTestScript::tick()
{
    if (nullptr == Animator())
        return;

    if (KEY_TAP(KEY::NUM1))
    {
        Animator()->Play(L"Armature|Walk", true, true, 1.f);
    }
    if (KEY_TAP(KEY::NUM2))
    {
        Animator()->Play(L"Armature|Wait", true, true, 1.f);
    }

    if (KEY_TAP(KEY::F))
    {
        LOG(Log, "%d", Animator()->IsFinish())
    }
}

void CAnimationTestScript::OnCollisionEnter(CCollider* _OtherCollider)
{
    LOG(Log, "OnCollisionEnter : %s", ToString(_OtherCollider->GetOwner()->GetName()).c_str());
}

void CAnimationTestScript::OnCollisionStay(CCollider* _OtherCollider)
{
    LOG(Log, "OnCollisionStay : %s", ToString(_OtherCollider->GetOwner()->GetName()).c_str());
}

void CAnimationTestScript::OnCollisionExit(CCollider* _OtherCollider)
{
    LOG(Log, "OnCollisionExit : %s", ToString(_OtherCollider->GetOwner()->GetName()).c_str());
}

void CAnimationTestScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    LOG(Log, "OnTriggerEnter : %s", ToString(_OtherCollider->GetOwner()->GetName()).c_str());
}

void CAnimationTestScript::OnTriggerStay(CCollider* _OtherCollider)
{
    LOG(Log, "OnTriggerStay : %s", ToString(_OtherCollider->GetOwner()->GetName()).c_str());
}

void CAnimationTestScript::OnTriggerExit(CCollider* _OtherCollider)
{
    LOG(Log, "OnTriggerExit : %s", ToString(_OtherCollider->GetOwner()->GetName()).c_str());
}
