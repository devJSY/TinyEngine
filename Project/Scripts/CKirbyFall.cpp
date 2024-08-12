#include "pch.h"
#include "CKirbyFall.h"

#include "CCameraController.h"
#include "CKirbyUnitScript.h"

CKirbyFall::CKirbyFall()
{
}

CKirbyFall::~CKirbyFall()
{
}

void CKirbyFall::tick()
{
    m_Acc += DT;

    if (!m_TimeStopExit && m_Acc > m_TimeStopDuration)
    {
        // @TODO DT 정상화

        m_TimeStopExit = true;
    }

    // State의 시간이 모두 지나면
    if (m_Acc > m_Duration)
    {
        // HP가 0이하인 경우
        if (PLAYERUNIT->GetCurInfo().HP <= 0.f)
        {
            // FLOW 매니저의 레벨 리셋 함수를 호출

        }
        // HP가 남아있는 경우 Respawn한다
        else
        {
            Vec3 RespawnPos = PLAYERCTRL->GetCheckPoint();
            RespawnPos.y += 30.f;

            // Player를 Respawn한다
            PLAYER->Transform()->SetWorldPos(RespawnPos);

            // Emissive, 무적 처리
            PLAYERFSM->SetEmissive(true, 2.f);
            PLAYERFSM->SetInvincible(true, 2.f);

            // State Change
            ChangeState(L"IDLE");

            PLAYER->CharacterController()->Move(Vec3(0.f, 0.1f, 0.f));

        }
    }

}

void CKirbyFall::Enter()
{
    // @TODO DT 0으로 바꿔주기


    // CameraSetting
    CCameraController* CamCtrl = CAMERACTRL;

    // Camera Shake
    CamCtrl->Shake(0.5f, 30.f, 30.f);

    GetOwner()->Animator()->Play(ANIMPREFIX("Damage"), true);

    PLAYERCTRL->LockInput();
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->SetLimitFallSpeed(true);

    // Kirby에게 데미지 주기
    PLAYERUNIT->GetDamage({DAMAGE_TYPE::NORMAL, Vec3(0.f, 0.f, 0.f), 10.f, 0.f, 0.f});

    m_Acc = 0.f;
    m_TimeStopDuration = 0.5f;
    m_Duration = 2.f;
    m_TimeStopExit = false;
}

void CKirbyFall::Exit()
{
    PLAYERCTRL->UnlockInput();
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetLimitFallSpeed(false);
}