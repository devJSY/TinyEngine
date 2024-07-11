#include "pch.h"
#include "CEngineTestScript.h"
#include <Engine\\CTransform.h>

CEngineTestScript::CEngineTestScript()
    : CScript(ENGINETESTSCRIPT)
{
}

CEngineTestScript::~CEngineTestScript()
{
}

void CEngineTestScript::begin()
{
}

void CEngineTestScript::tick()
{
    // GamePlayStatic::DrawDebugLine(GetOwner()->Transform()->GetWorldPos(), GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT), 100.f,
    //                               Vec3(1.f, 1.f, 0.f), true);

    // CGameObject* pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Target");
    // if (pTarget)
    //{
    //     GetOwner()->Transform()->LookAt(pTarget->Transform()->GetWorldPos());
    // }
}

void CEngineTestScript::OnCollisionEnter(CCollider* _OtherCollider)
{
}

void CEngineTestScript::OnCollisionStay(CCollider* _OtherCollider)
{
}

void CEngineTestScript::OnCollisionExit(CCollider* _OtherCollider)
{
}

void CEngineTestScript::OnTriggerEnter(CCollider* _OtherCollider)
{
}

void CEngineTestScript::OnTriggerStay(CCollider* _OtherCollider)
{
}

void CEngineTestScript::OnTriggerExit(CCollider* _OtherCollider)
{
}

void CEngineTestScript::SaveToLevelFile(FILE* _File)
{
}

void CEngineTestScript::LoadFromLevelFile(FILE* _File)
{
}
