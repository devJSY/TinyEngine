#include "pch.h"
#include "ElfilisDimensionSet.h"
#include "ElfilisDimensionSpear.h"
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

ElfilisDimensionSet::ElfilisDimensionSet()
    : CScript(ELFILISDIMENSIONSET)
    , m_Step(0)
    , m_AccTime(0.f)
{
    Ptr<CPrefab> DimensionSpear = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ElfilisDimensionSpear.pref");

    if (DimensionSpear != nullptr)
    {
        for (int i = 0; i < 5; ++i)
        {
            m_Spear[i] = DimensionSpear->Instantiate();
            m_SpearScript[i] = m_Spear[i]->GetScript<ElfilisDimensionSpear>();
        }
    }
}

ElfilisDimensionSet::~ElfilisDimensionSet()
{
}

void ElfilisDimensionSet::begin()
{
    for (int i = 0; i < 5; ++i)
    {
        if (m_SpearScript[i])
        {
            Vec3 Pos = m_InitPos + m_ArrDir * 100.f * i;
            Vec3 Rot = -m_ArrDir;
            Rot.y += XMConvertToRadians(GetRandomfloat(-30.f, 30.f));

            m_SpearScript[i]->SetInitPos(Pos);
            m_SpearScript[i]->SetInitRot(Rot);
        }
    }
}

void ElfilisDimensionSet::tick()
{
    m_AccTime += DT;

    switch (m_Step)
    {
    case 0:
        Open();
        break;
    case 1:
        Ready();
        break;
    case 2:
        Attack();
        break;
    case 3:
        AttackEnd();
        break;
    case 4:
        Close();
        break;
    }
}

void ElfilisDimensionSet::Open()
{
    if (m_AccTime < 0.2f)
    {
        // scaling
    }
}

void ElfilisDimensionSet::Ready()
{
}

void ElfilisDimensionSet::Attack()
{
}

void ElfilisDimensionSet::AttackEnd()
{
}

void ElfilisDimensionSet::Close()
{
}

UINT ElfilisDimensionSet::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT ElfilisDimensionSet::LoadFromLevelFile(FILE* _File)
{
    return 0;
}