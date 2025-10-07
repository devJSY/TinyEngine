#include "pch.h"
#include "CEngineTestScript.h"
#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CSoundMgr.h>

CEngineTestScript::CEngineTestScript()
    : CScript(ENGINETESTSCRIPT)
    , TestParam1(1.f)
    , TestParam2(1.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &TestParam1, "TestParam1");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &TestParam2, "TestParam2");
}

CEngineTestScript::CEngineTestScript(const CEngineTestScript& origin)
    : CScript(origin)
    , TestParam1(origin.TestParam1)
    , TestParam2(origin.TestParam2)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &TestParam1, "TestParam1");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &TestParam2, "TestParam2");
}

CEngineTestScript::~CEngineTestScript()
{
}

void CEngineTestScript::begin()
{
}

void CEngineTestScript::tick()
{
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

UINT CEngineTestScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    fwrite(&TestParam1, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CEngineTestScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    fread(&TestParam1, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}
