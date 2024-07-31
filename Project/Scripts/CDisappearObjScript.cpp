#include "pch.h"
#include "CDisappearObjScript.h"

CDisappearObjScript::CDisappearObjScript()
    : CScript(DISAPPEAROBJSCRIPT)
    , m_fAccTime(0.f)
    , m_fBreakTime(0.f)
    , m_fCreateTime(0.f)
    , m_bExist(true)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fBreakTime, "Break Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fCreateTime, "Create Time");
}

CDisappearObjScript::CDisappearObjScript(const CDisappearObjScript& Origin)
    : CScript(Origin)
    , m_fAccTime(0.f)
    , m_fBreakTime(0.f)
    , m_fCreateTime(0.f)
    , m_bExist(true)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fBreakTime, "Break Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fCreateTime, "Create Time");
}

CDisappearObjScript::~CDisappearObjScript()
{
}

void CDisappearObjScript::tick()
{
    if (!m_bExist)
    {
        m_fAccTime += DT;
        if (m_fAccTime >= m_fCreateTime)
        {
            BoxCollider()->SetEnabled(true);
            m_fAccTime = 0.f;
            m_bExist = true;
        }
    }
}

void CDisappearObjScript::OnTriggerStay(CCollider* _OtherCollider)
{
    CGameObject* Obj = _OtherCollider->GetOwner();
    if (L"Body Collider" == Obj->GetName())
    {
        m_fAccTime += DT;

        if (m_fAccTime >= m_fBreakTime)
        {
            // TODO Kerby 상태 변경 해주기
            BoxCollider()->SetEnabled(false);
        }
    }
}

void CDisappearObjScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* Obj = _OtherCollider->GetOwner();
    if (L"Body Collider" == Obj->GetName())
    {
        m_fAccTime = 0.f;
    }
}

UINT CDisappearObjScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fBreakTime, sizeof(float), 1, _File);
    fwrite(&m_fCreateTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CDisappearObjScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fBreakTime, sizeof(float), 1, _File);
    fread(&m_fCreateTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}