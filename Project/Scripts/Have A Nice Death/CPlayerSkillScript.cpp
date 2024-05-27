#include "pch.h"
#include "CPlayerSkillScript.h"
#include "CEnemyScript.h"
#include "CPlayerCameraScript.h"

CPlayerSkillScript::CPlayerSkillScript()
    : CScript(PLAYERSKILLSCRIPT)
    , m_ATK(0)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_ATK, "ATK");
}

CPlayerSkillScript::CPlayerSkillScript(const CPlayerSkillScript& origin)
    : CScript(origin)
    , m_ATK(origin.m_ATK)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_ATK, "ATK");
}

CPlayerSkillScript::~CPlayerSkillScript()
{
}

void CPlayerSkillScript::begin()
{
    if (nullptr != BoxCollider2D())
        BoxCollider2D()->SetEnabled(true);
}

void CPlayerSkillScript::tick()
{
}

void CPlayerSkillScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    CEnemyScript* EnemyScript = _OtherCollider->GetOwner()->GetScript<CEnemyScript>();
    if (nullptr == EnemyScript)
        return;

    Vec3 SkillPos = Transform()->GetWorldPos();
    Vec3 EnemyPos = _OtherCollider->GetOwner()->Transform()->GetWorldPos();

    SkillPos.z = 0;
    EnemyPos.z = 0;

    Vec3 Dir = EnemyPos - SkillPos;
    Dir.Normalize();

    if (EnemyScript->TakeHit(m_ATK, Dir))
    {
    }
}

void CPlayerSkillScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_ATK, sizeof(int), 1, _File);
}

void CPlayerSkillScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_ATK, sizeof(int), 1, _File);
}
