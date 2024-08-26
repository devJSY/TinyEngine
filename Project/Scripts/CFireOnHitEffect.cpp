#include "pch.h"
#include "CFireOnHitEffect.h"


CFireOnHitEffect::CFireOnHitEffect()
    : CScript(FIREONHITEFFECT)
    , m_Step((UINT)FireEffect_Step::LV0)
    , m_AccFireNumber(0)
    , m_RequiredFireNumber(5)
    , m_Acc(0.f)
    , m_Duration(4.f)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_Step, "Fire Level(0~2)");
    AddScriptParam(SCRIPT_PARAM::INT, &m_RequiredFireNumber, "Required Fire Number");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
}

CFireOnHitEffect::CFireOnHitEffect(const CFireOnHitEffect& Origin)
    : CScript(FIREONHITEFFECT)
    , m_Step((UINT)FireEffect_Step::LV0)
    , m_AccFireNumber(0)
    , m_RequiredFireNumber(5)
    , m_Acc(0.f)
    , m_Duration(4.f)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_Step, "Fire Level(0~2)");
    AddScriptParam(SCRIPT_PARAM::INT, &m_RequiredFireNumber, "Required Fire Number");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
}

CFireOnHitEffect::~CFireOnHitEffect()
{
}

void CFireOnHitEffect::begin()
{
}

void CFireOnHitEffect::tick()
{
}

void CFireOnHitEffect::OnTriggerEnter(CCollider* _OtherCollider)
{
}

UINT CFireOnHitEffect::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CFireOnHitEffect::LoadFromLevelFile(FILE* _File)
{
    return 0;
}

