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
    // 전체 글라이딩 시간을 위해 FSM에 Gliding 시간을 더해준다.
    PLAYERFSM->AddGlidingTime(DT);

    // 중력값 조절
    float CurGravity = PLAYERCTRL->GetGravity();
    if (CurGravity < 10.f)
    {
        CurGravity += 100.f * DT;
        PLAYERCTRL->SetGravity(CurGravity);
    }

    float CurGlidingTime = PLAYERFSM->GetCurGlidingTime();

    // 글라이딩 시간이 끝났거나 공격키를 누르지 않으면
    if (CurGlidingTime > PLAYERFSM->GetGlidingDuration() || (KEY_RELEASED(KEY_ATK)||KEY_NONE(KEY_ATK)))
    {
        ChangeState(L"JUMP_FALL");
    }
}

void CKirbyBurningEnd::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("BurningEnd"), false, false, 1.5f);

    // Movement
    PLAYERCTRL->LockJump();
    PLAYERCTRL->SetForwardMode(true);
    m_SaveRotSpeed = PLAYERCTRL->GetRotSpeed();
    PLAYERCTRL->SetRotSpeed(0.5f);

    m_SaveSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(13.f);

    //  무적 상태
    PLAYERFSM->SetInvincible(true);
}

void CKirbyBurningEnd::Exit()
{
    // Movement
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetForwardMode(false);
    PLAYERCTRL->SetRotSpeed(m_SaveRotSpeed);

    PLAYERCTRL->SetGravity(-20.f);
    PLAYERCTRL->SetSpeed(m_SaveSpeed);

    // MeshData 돌려주기
    PLAYER->MeshRender()->SetMeshData(CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\Kirby.mdat", L"meshdata\\Kirby.mdat"));

    PLAYER->MeshRender()->SetMaterial(nullptr, 0);
    PLAYER->MeshRender()->SetMaterial(nullptr, 2);
    PLAYER->MeshRender()->SetMaterial(nullptr, 4);
    PLAYER->MeshRender()->SetMaterial(nullptr, 6);
    PLAYER->MeshRender()->SetMaterial(nullptr, 7);
    PLAYER->MeshRender()->SetMaterial(nullptr, 8);

    // 모자 다시 보이게하기
    Ptr<CMaterial> HatMat =
        CAssetMgr::GetInst()->Load<CMaterial>(L"material\\KiryDragonHat_DragonFireC.mtrl", L"material\\KiryDragonHat_DragonFireC.mtrl");
    PLAYERFSM->GetCurHat()->MeshRender()->SetMaterial(HatMat, 0);

    //  무적 상태
    PLAYERFSM->SetInvincible(false);
}

