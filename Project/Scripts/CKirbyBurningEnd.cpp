#include "pch.h"
#include "CKirbyBurningEnd.h"

CKirbyBurningEnd::CKirbyBurningEnd()
    : m_SaveRotSpeed(0.f)
    , m_SaveSpeed(0.f)
{
}

CKirbyBurningEnd::~CKirbyBurningEnd()
{
}
void CKirbyBurningEnd::tick()
{
    // ��ü �۶��̵� �ð��� ���� FSM�� Gliding �ð��� �����ش�.
    PLAYERFSM->AddGlidingTime(DT);

    // �߷°� ����
    float CurGravity = PLAYERCTRL->GetGravity();
    if (CurGravity < 10.f)
    {
        CurGravity += 100.f * DT;
        PLAYERCTRL->SetGravity(CurGravity);
    }

    float CurGlidingTime = PLAYERFSM->GetCurGlidingTime();

    // �۶��̵� �ð��� �����ų� ����Ű�� ������ ������
    if (CurGlidingTime > PLAYERFSM->GetGlidingDuration() || (KEY_RELEASED(KEY_ATK)||KEY_NONE(KEY_ATK)))
    {
        ChangeState(L"JUMP_FALL");
    }
}

void CKirbyBurningEnd::Enter()
{
    // �ִϸ��̼� ���
    //PLAYER->Animator()->Play(KIRBYANIM(L"BurningEnd"), false, false, 1.5f);

    PLAYER->Animator()->Play(KIRBYANIM(L"Wait"), false, false, 1.5f);

    // Movement
    PLAYERCTRL->LockJump();
    PLAYERCTRL->SetForwardMode(true);
    m_SaveRotSpeed = PLAYERCTRL->GetRotSpeed();
    PLAYERCTRL->SetRotSpeed(0.5f);

    m_SaveSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(13.f);
}

void CKirbyBurningEnd::Exit()
{
    // Movement
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetForwardMode(false);
    PLAYERCTRL->SetRotSpeed(m_SaveRotSpeed);

    PLAYERCTRL->SetGravity(-20.f);
    PLAYERCTRL->SetSpeed(m_SaveSpeed);
}
