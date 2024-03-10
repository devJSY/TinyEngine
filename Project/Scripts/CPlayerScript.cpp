#include "pch.h"
#include "CPlayerScript.h"
#include "CShockWaveScript.h"
#include <Engine\\CAnim.h>

CPlayerScript::CPlayerScript()
    : CScript(PLAYERSCRIPT)
    , m_Speed(10.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
    /* m_ShockWave = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ShockWavePrefab.pref", L"prefab\\ShockWavePrefab.pref");

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

     Animator2D()->Play(CurAnimName, true);*/
}

void CPlayerScript::tick()
{
    /*  Vec3 vPos = Transform()->GetRelativePos();
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
      }*/

    //// °øÆ÷Åº »ý¼º
    // if (KEY_TAP(KEY::Q))
    //{
    //     if (nullptr != m_ShockWave)
    //     {
    //         CGameObject* pShockWave = m_ShockWave->Instantiate();
    //         pShockWave->Transform()->SetRelativePos(Transform()->GetRelativePos());
    //         GamePlayStatic::SpawnGameObject(pShockWave, 14);
    //     }
    // }

    // Transform()->SetRelativePos(vPos);
    // Transform()->SetRelativeRotation(vRot);

    if (nullptr != Rigidbody2D())
    {
        /*if (KEY_PRESSED(KEY::UP))
        {
            Rigidbody2D()->AddForce(Vec2(0.f, 5.f), ForceMode2D::Impulse);
        }

        if (KEY_PRESSED(KEY::DOWN))
        {
            Rigidbody2D()->AddForce(Vec2(0.f, -5.f), ForceMode2D::Impulse);
        }

        if (KEY_PRESSED(KEY::LEFT))
        {
            Rigidbody2D()->AddForce(Vec2(-5.f, 0.f), ForceMode2D::Impulse);
        }

        if (KEY_PRESSED(KEY::RIGHT))
        {
            Rigidbody2D()->AddForce(Vec2(5.f, 0.f), ForceMode2D::Impulse);
        }*/

        Vec2 vel = Rigidbody2D()->GetVelocity();

        if (KEY_PRESSED(KEY::UP))
        {
            vel.y += m_Speed * DT;
        }

        if (KEY_PRESSED(KEY::DOWN))
        {
            vel.y -= m_Speed * DT;
        }

        if (KEY_PRESSED(KEY::LEFT))
        {
            vel.x -= m_Speed * DT;
        }

        if (KEY_PRESSED(KEY::RIGHT))
        {
            vel.x += m_Speed * DT;
        }

        Rigidbody2D()->SetVelocity(vel);

        if (KEY_TAP(KEY::SPACE))
        {
            Rigidbody2D()->AddForce(Vec2(0.f, 5.f), ForceMode2D::Impulse);
        }

        if (KEY_TAP(KEY::O))
        {
            CGameObject* pObj = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Test.pref", L"prefab\\Test.pref")->Instantiate();
            pObj->Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(0.f, 5.f, 0.f));
            pObj->AddComponent(CScriptMgr::GetScript(GROUNDSCRIPT));
            GamePlayStatic::SpawnGameObject(pObj, 0);
        }
    }
}

void CPlayerScript::OnCollisionEnter(CCollider2D* _OtherCollider)
{
}

void CPlayerScript::OnCollisionStay(CCollider2D* _OtherCollider)
{
}

void CPlayerScript::OnCollisionExit(CCollider2D* _OtherCollider)
{
}

void CPlayerScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
}

void CPlayerScript::OnTriggerStay(CCollider2D* _OtherCollider)
{
}

void CPlayerScript::OnTriggerExit(CCollider2D* _OtherCollider)
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
