#include "pch.h"
#include "CPlayerScript.h"
#include "CShockWaveScript.h"
#include <Engine\\CAnim.h>

CPlayerScript::CPlayerScript()
    : CScript(PLAYERSCRIPT)
    , m_Speed(10.f)
    , m_Force(1.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Force, "Force");
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
    if (MeshRender())
        MeshRender()->CreateDynamicMaterial();

    if (Animator2D())
    {
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Acquisition.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Acquisition_Curse.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_AerialDownAttack.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_BookAttack_01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_BookAttack_02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_BookAttack_03.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_BossRitual_Intro.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Bump.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Combo_Stand.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboAerial_01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboAerial_01_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboAerial_02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboAerial_02_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboAerial_03.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboAerial_03_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboEvilHands.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboEvilHands_Up.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboKatana.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboKatanas_Ult.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboKatanas_Ult_Init.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboKatanas_Ult_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_01_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_02_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_03.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_03_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_04.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_04_Rest.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ComboMove_Jump.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Concentrate.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Concentrate_Start.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Dash.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Disappear.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Elevator_End.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Elevator_Enter.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Elevator_Out.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_EvilBirds.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_EvilBirds_Up.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Glide.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Hit.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Idle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_IdleToRun.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_IdleUturn.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Jump_Falling.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Jump_Landing.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Jump_Start.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_JumpingAttack.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_OneWayDown.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_OneWayUp.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_PowerUp.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_PowerUp_01.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_PowerUp_02.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Ritual_End_Boss.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Run.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_RunToIdle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_RunUturn.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Spawn_Lobby.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Teleport.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Waiting.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_Wallgrab_Idle.anim");
        Animator2D()->LoadAnimation(L"AnimData\\Player\\LD_ZipUp.anim");

        Animator2D()->Play(L"LD_Acquisition");
    }
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
        Vec3 pos = Transform()->GetRelativePos();

        if (KEY_PRESSED(KEY::UP))
        {
            pos.y += m_Speed * DT;
        }

        if (KEY_PRESSED(KEY::DOWN))
        {
            pos.y -= m_Speed * DT;
        }

        if (KEY_PRESSED(KEY::LEFT))
        {
            pos.x -= m_Speed * DT;
        }

        if (KEY_PRESSED(KEY::RIGHT))
        {
            pos.x += m_Speed * DT;
        }

        Transform()->SetRelativePos(pos);

        if (KEY_TAP(KEY::SPACE))
        {
            Rigidbody2D()->AddForce(Vec2(0.f, m_Force), ForceMode2D::Impulse);
        }

        if (KEY_TAP(KEY::TAB))
        {
            Rigidbody2D()->AddTorque(m_Force, ForceMode2D::Impulse);
        }
    }
}

void CPlayerScript::OnCollisionEnter(CCollider2D* _OtherCollider)
{
    LOG(Log, "OnCollisionEnter");
}

void CPlayerScript::OnCollisionStay(CCollider2D* _OtherCollider)
{
    LOG(Log, "OnCollisionStay");
}

void CPlayerScript::OnCollisionExit(CCollider2D* _OtherCollider)
{
    LOG(Log, "OnCollisionExit");
}

void CPlayerScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    LOG(Log, "OnTriggerEnter");
}

void CPlayerScript::OnTriggerStay(CCollider2D* _OtherCollider)
{
    LOG(Log, "OnTriggerStay");
}

void CPlayerScript::OnTriggerExit(CCollider2D* _OtherCollider)
{
    LOG(Log, "OnTriggerExit");
}

void CPlayerScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Speed, sizeof(float), 1, _File);
    fwrite(&m_Force, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Speed, sizeof(float), 1, _File);
    fread(&m_Force, sizeof(float), 1, _File);
}
