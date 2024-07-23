#include "pch.h"
#include "CKirbyUnitScript.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CState.h"

CKirbyUnitScript::CKirbyUnitScript()
    : CUnitScript(KIRBYUNITSCRIPT)
{
    UnitInfo KirbyInfo = {
        100.f, // HP
        100.f, // MaxHP
        10.f,  // Speed
        10.f,  // Rotation Speed
        10.f,  // JumpPower
    };
    SetInitInfo(KirbyInfo);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.HP, "[Init] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.MAXHP, "[Init] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.Speed, "[Init] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.RotationSpeed, "[Init] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.JumpPower, "[Init] Jump Power");

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
        10.f,  // JumpPower
    };
    SetInitInfo(KirbyInfo);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.HP, "[Init] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.MAXHP, "[Init] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.Speed, "[Init] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.RotationSpeed, "[Init] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.JumpPower, "[Init] Jump Power");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "[Current] HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "[Current] HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "[Current] Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "[Current] Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "[Current] Jump Power");
}

CKirbyUnitScript::~CKirbyUnitScript()
{
}

void CKirbyUnitScript::tick()
{
    ClearHitDir();
    m_PrevInfo = m_CurInfo;

    // update damage
    float NewDamage = DamageProc();
    if (NewDamage > 0.f)
    {
        if (PLAYERFSM->IsInvincible())
        {
            m_HitHistory.clear();
        }
        else if (PLAYERFSM->GetCurState()->GetName() == L"GUARD")
        {
            m_CurInfo.HP -= NewDamage * 0.25f;
        }
        else
        {
            m_CurInfo.HP -= NewDamage;
            SetHitDirHorizen();
            PLAYERFSM->ChangeState(L"DAMAGE");
        }
    }

    // HP Ã³¸®
    if (m_CurInfo.HP <= 0.f)
    {
        m_CurInfo.HP = 0.f;
        //PLAYERFSM->ChangeState(L"DEATH");
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

UINT CKirbyUnitScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CUnitScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CKirbyUnitScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CUnitScript::LoadFromLevelFile(_File);

    // if (m_CurInfo.MAXHP == 0.f)
    //{
    // UnitInfo KirbyInfo = {
    //     100.f, // HP
    //     100.f, // MaxHP
    //     10.f,  // Speed
    //     10.f,  // Rotation Speed
    //     10.f,  // JumpPower
    // };
    //     SetInitInfo(KirbyInfo);
    // }

    return MemoryByte;
}
