#include "pch.h"
#include "CPlayerScript.h"
#include "CShockWaveScript.h"
#include <Engine\\CAnim.h>

CPlayerScript::CPlayerScript()
    : CScript(PLAYERSCRIPT)
    , m_Speed(100.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
    m_ShockWave = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ShockWavePrefab.pref", L"prefab\\ShockWavePrefab.pref");

    if (nullptr == Animator2D())
        return;

    CAnim* CurAnim = Animator2D()->GetCurAnim();
    wstring CurAnimName = wstring();
    if (nullptr != CurAnim)
        CurAnimName = CurAnim->GetName();

    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\dodge_back.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\dodge_backward.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\dodge_forward.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\dodge_front.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_back.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_back_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_backward.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_forward.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_forward_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_front.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_front_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_back.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_back_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_backward.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_backward_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_doorway.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_forward.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_forward_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_front.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_front_hands.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\select_choose.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\select_idle.anim");
    Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\select_smoking_idle.anim");

    Animator2D()->Play(CurAnimName, true);
}

void CPlayerScript::tick()
{
    Vec3 vPos = Transform()->GetRelativePos();
    Vec3 vRot = Transform()->GetRelativeRotation();

    if (KEY_PRESSED(KEY::UP))
    {
        vPos.y += DT * m_Speed;
    }

    if (KEY_PRESSED(KEY::DOWN))
    {
        vPos.y -= DT * m_Speed;
    }

    if (KEY_PRESSED(KEY::LEFT))
    {
        vPos.x -= DT * m_Speed;
    }

    if (KEY_PRESSED(KEY::RIGHT))
    {
        vPos.x += DT * m_Speed;
    }

    // °øÆ÷Åº »ý¼º
    if (KEY_TAP(KEY::Q))
    {
        if (nullptr != m_ShockWave)
        {
            CGameObject* pShockWave = m_ShockWave->Instantiate();
            pShockWave->Transform()->SetRelativePos(Transform()->GetRelativePos());
            GamePlayStatic::SpawnGameObject(pShockWave, 14);
        }
    }

    Transform()->SetRelativePos(vPos);
    Transform()->SetRelativeRotation(vRot);
}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Speed, sizeof(float), 1, _File);
}
