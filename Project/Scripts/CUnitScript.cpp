#include "pch.h"
#include "CUnitScript.h"

CUnitScript::CUnitScript(UINT _Type)
    : CScript(_Type)
    , m_PrevInfo{}
    , m_CurInfo{}
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "MAXHP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "CURHP");
}

CUnitScript::~CUnitScript()
{
}


void CUnitScript::tick()
{
    m_PrevInfo = m_CurInfo;

    // 현재 프레임에 받는 데미지들을 업데이트
    DamageProc();
}

void CUnitScript::GetDamage(UnitHit _Damage)
{
    m_HitHistory.push_back(_Damage);
}

void CUnitScript::DamageProc()
{
    float CurDamage = 0.f;

    std::list<UnitHit>::iterator iter = m_HitHistory.begin();

    while (iter != m_HitHistory.end())
    {
        switch (iter->Type)
        {
        case DAMAGE_TYPE::NORMAL:
            CurDamage += iter->Damage;
            iter = m_HitHistory.erase(iter);
            break;

        case DAMAGE_TYPE::DOT:
        {
            if (iter->Acc >= DOT_TERM)
            {
                CurDamage += iter->Damage;
                iter->Acc -= DOT_TERM;
                iter->Duration -= DOT_TERM;
            }

            if (iter->Duration <= 0.f)
            {
                iter = m_HitHistory.erase(iter);
            }
        }
            break;

        default:
            break;
        }
    }

    m_CurInfo.HP -= CurDamage;
}

void CUnitScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_CurInfo, sizeof(UnitInfo), 1, _File);
}

void CUnitScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_CurInfo, sizeof(UnitInfo), 1, _File);
}

