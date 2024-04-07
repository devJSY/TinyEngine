#include "pch.h"
#include "CEnemyScript.h"

CEnemyScript::CEnemyScript(UINT _ScriptType)
    : CScript(_ScriptType)
    , m_Dir(DIRECTION_TYPE::RIGHT)
    , m_MaxLife(0)
    , m_CurLife(0)
    , m_Speed(0.f)
    , m_ATK(0)
    , m_AttackRange(0.f)
    , m_pTarget(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_MaxLife, "Max Life");
    AddScriptParam(SCRIPT_PARAM::INT, &m_CurLife, "Current Life");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::INT, &m_ATK, "ATK");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_AttackRange, "Attack Range");
}

CEnemyScript::CEnemyScript(const CEnemyScript& origin)
    : CScript(origin)
    , m_Dir(origin.m_Dir)
    , m_MaxLife(origin.m_MaxLife)
    , m_CurLife(origin.m_CurLife)
    , m_Speed(origin.m_Speed)
    , m_ATK(origin.m_ATK)
    , m_AttackRange(origin.m_AttackRange)
    , m_pTarget(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_MaxLife, "Max Life");
    AddScriptParam(SCRIPT_PARAM::INT, &m_CurLife, "Current Life");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::INT, &m_ATK, "ATK");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_AttackRange, "Attack Range");
}

CEnemyScript::~CEnemyScript()
{
}

void CEnemyScript::tick()
{
    GamePlayStatic::DrawDebugCircle(Transform()->GetWorldPos(), m_AttackRange, Vec3(1.f, 1.f, 0.f), false);
}

void CEnemyScript::Walking()
{
    Vec2 vel = Rigidbody2D()->GetVelocity();

    if (DIRECTION_TYPE::LEFT == m_Dir)
        vel.x = -m_Speed;
    else
        vel.x = m_Speed;

    Rigidbody2D()->SetVelocity(vel);
}

void CEnemyScript::RotateTransform()
{
    if (DIRECTION_TYPE::LEFT == m_Dir)
        Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
    else
        Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
}

void CEnemyScript::SetHitBox(bool _Enable, const wstring& _HitBoxName)
{
    const vector<CGameObject*>& vecChild = GetOwner()->GetChildObject();
    for (size_t i = 0; i < vecChild.size(); i++)
    {
        CEnemyHitBoxScript* pHitBox = vecChild[i]->GetScript<CEnemyHitBoxScript>();
        if (nullptr == pHitBox)
            continue;

        // _HitBoxName 이 입력 되지 않은 경우 모든 HitBox에 적용
        // _HitBoxName 이 입력 된경우 해당 이름의 HitBox만 적용
        if (!_HitBoxName.empty() && pHitBox->GetOwner()->GetName() != _HitBoxName)
            continue;

        pHitBox->SetEnemy(this);
        pHitBox->SetEnabled(_Enable);
    }
}

void CEnemyScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_MaxLife, sizeof(int), 1, _File);
    fwrite(&m_CurLife, sizeof(int), 1, _File);
    fwrite(&m_Speed, sizeof(float), 1, _File);
    fwrite(&m_ATK, sizeof(int), 1, _File);
    fwrite(&m_AttackRange, sizeof(float), 1, _File);
}

void CEnemyScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_MaxLife, sizeof(int), 1, _File);
    fread(&m_CurLife, sizeof(int), 1, _File);
    fread(&m_Speed, sizeof(float), 1, _File);
    fread(&m_ATK, sizeof(int), 1, _File);
    fread(&m_AttackRange, sizeof(float), 1, _File);
}
