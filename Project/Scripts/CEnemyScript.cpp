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
    , m_ExclamationMarkPref(nullptr)
    , m_FXGhostPref(nullptr)
    , m_FXHealDeflagrationPref(nullptr)
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
    , m_ExclamationMarkPref(origin.m_ExclamationMarkPref)
    , m_FXGhostPref(origin.m_FXGhostPref)
    , m_FXHealDeflagrationPref(origin.m_FXHealDeflagrationPref)
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

void CEnemyScript::begin()
{
    m_ExclamationMarkPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Exclamation_Mark.pref", L"prefab\\Exclamation_Mark.pref");
    m_FXGhostPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FX_Ghost.pref", L"prefab\\FX_Ghost.pref");
    m_FXHealDeflagrationPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FX_Heal_Deflagration.pref", L"prefab\\FX_Heal_Deflagration.pref");
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

void CEnemyScript::SpawnExclamationMark(float _Height)
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    int EffectIdx = pCurLevel->FindLayerIndexByName(L"Effect");
    if (-1 == EffectIdx)
        EffectIdx = 0;

    CGameObject* pExclamationMarkObj = m_ExclamationMarkPref->Instantiate();
    pExclamationMarkObj->Transform()->SetRelativePos(Vec3(0.f, _Height, 0.f));
    GamePlayStatic::SpawnGameObject(pExclamationMarkObj, EffectIdx);
    GamePlayStatic::AddChildObject(GetOwner(), pExclamationMarkObj);
}

void CEnemyScript::SpawnFXGhost()
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    int EffectIdx = pCurLevel->FindLayerIndexByName(L"Effect");
    if (-1 == EffectIdx)
        EffectIdx = 0;

    float Theta = XM_2PI / 6.f;
    for (int i = 0; i < 6; i++)
    {
        CGameObject* pFXGhostObj = m_FXGhostPref->Instantiate();

        if (0 == i || 3 == i)
            pFXGhostObj->Animator2D()->Play(L"Ghost01", false);
        else if (1 == i || 4 == i)
            pFXGhostObj->Animator2D()->Play(L"Ghost02", false);
        else if (2 == i || 5 == i)
            pFXGhostObj->Animator2D()->Play(L"Ghost03", false);

        pFXGhostObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, Theta * i));

        Vec3 Dir = Vec3::Transform(Vec3(0.f, 1.f, 0.f), Matrix::CreateRotationZ(Theta * i));
        Dir.Normalize();
        Dir *= 200.f;
        pFXGhostObj->Transform()->SetRelativePos(Transform()->GetRelativePos() + Dir);

        GamePlayStatic::SpawnGameObject(pFXGhostObj, EffectIdx);
    }
}

void CEnemyScript::SpawnFXHealDeflagration()
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    int EffectIdx = pCurLevel->FindLayerIndexByName(L"Effect");
    if (-1 == EffectIdx)
        EffectIdx = 0;

    CGameObject* FXHealDeflagrationObj = m_FXHealDeflagrationPref->Instantiate();
    FXHealDeflagrationObj->Transform()->SetRelativePos(Transform()->GetWorldPos());

    GamePlayStatic::SpawnGameObject(FXHealDeflagrationObj, EffectIdx);
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
