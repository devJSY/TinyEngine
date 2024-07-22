#include "pch.h"
#include "CUnitScript.h"

CUnitScript::CUnitScript(UINT _Type)
    : CScript(_Type)
    , m_InitInfo{}
    , m_PrevInfo{}
    , m_CurInfo{}
{
}

CUnitScript::CUnitScript(const CUnitScript& _Origin)
    : CScript(_Origin)
    , m_InitInfo{}
    , m_PrevInfo{}
    , m_CurInfo(_Origin.m_CurInfo)
{
}

CUnitScript::~CUnitScript()
{
}

void CUnitScript::begin()
{
    m_CurInfo = m_InitInfo;
}

void CUnitScript::tick()
{
    ClearHitDir();
    m_PrevInfo = m_CurInfo;

    // 현재 프레임에 받는 데미지들을 업데이트
    float NewDamage = DamageProc();
    if (NewDamage > 0.f)
    {
        m_CurInfo.HP -= NewDamage;
    }

    if (m_CurInfo.HP < 0.f)
    {
        m_CurInfo.HP = 0.f;
    }
    
    if (m_CurInfo.HP > m_CurInfo.MAXHP)
    {
        m_CurInfo.HP = m_CurInfo.MAXHP;
    }
}

void CUnitScript::GetDamage(UnitHit _Damage)
{
    m_HitHistory.push_back(_Damage);
}

float CUnitScript::DamageProc()
{
    float CurDamage = 0.f;

    std::list<UnitHit>::iterator iter = m_HitHistory.begin();

    while (iter != m_HitHistory.end())
    {
        switch (iter->Type)
        {
        case DAMAGE_TYPE::NORMAL:
        {
            CurDamage += iter->Damage;
            m_HitDir += iter->HitDir;
            iter = m_HitHistory.erase(iter);
        }
            break;

        case DAMAGE_TYPE::DOT: {
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
        }
    }

    return CurDamage;
}

void CUnitScript::SetHitDirHorizen()
{
    m_HitDir.y = 0.f;
    m_HitDir.Normalize();
}

void CUnitScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_InitInfo, sizeof(UnitInfo), 1, _File);
}

void CUnitScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_InitInfo, sizeof(UnitInfo), 1, _File);
    m_CurInfo = m_InitInfo;
}
