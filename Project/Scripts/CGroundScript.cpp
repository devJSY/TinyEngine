#include "pch.h"
#include "CGroundScript.h"

CGroundScript::CGroundScript()
    : CScript(GROUNDSCRIPT)
{
}

CGroundScript::~CGroundScript()
{
}

void CGroundScript::tick()
{
}

void CGroundScript::OnCollisionEnter(CCollider2D* _OtherCollider)
{
    LOG(Log, "OnCollisionEnter");
}

void CGroundScript::OnCollisionStay(CCollider2D* _OtherCollider)
{
    LOG(Log, "OnCollisionStay");
}

void CGroundScript::OnCollisionExit(CCollider2D* _OtherCollider)
{
    LOG(Log, "OnCollisionExit");
}

void CGroundScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    LOG(Log, "OnTriggerEnter");
}

void CGroundScript::OnTriggerStay(CCollider2D* _OtherCollider)
{
    LOG(Log, "OnTriggerStay");
}

void CGroundScript::OnTriggerExit(CCollider2D* _OtherCollider)
{
    LOG(Log, "OnTriggerExit");
}
