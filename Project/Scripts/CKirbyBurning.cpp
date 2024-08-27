#include "pch.h"
#include "CKirbyBurning.h"

CKirbyBurning::CKirbyBurning()
    : m_SaveRotSpeed(0.f)
    , m_Duration(0.5f)
    , m_Acc(0.f)
    , m_SaveSpeed(0.f)
{
}

CKirbyBurning::~CKirbyBurning()
{
}

void CKirbyBurning::tick()
{
    m_Acc += DT;

    // 전체 글라이딩 시간을 위해 FSM에 Gliding 시간을 더해준다.
    PLAYERFSM->AddGlidingTime(DT);

    // 중력값 조절
    float CurGravity = PLAYERCTRL->GetGravity();
    if (CurGravity < 1.f)
    {
        CurGravity += 100.f * DT;
        PLAYERCTRL->SetGravity(CurGravity);
    }

    // 현재스테이트의 제한시간이 넘어갔거나, 땅에 닿았을 경우 BurningEnd
    if (m_Acc > m_Duration)
    {
        if (KEY_RELEASED(KEY_ATK))
        {
            ChangeState(L"JUMP_FALL");
        }
        else
        {
            ChangeState(L"BURNING_END");
        }
    }

    Vec3 Raystart = GetOwner()->Transform()->GetWorldPos();
    Raystart.y += 20.f;

    Vec3 RayDir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);

    float RayLength = GetOwner()->CharacterController()->GetRadius() * 20.f + 5.f;

    static vector<wstring> FireWallCollision{L"World Static"};
    RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Raystart, RayDir, RayLength, FireWallCollision);

    if (Hit.pCollisionObj != nullptr)
    {
        Vec3 Normal = Hit.Normal;
        Normal.y = 0.f;

        if (Normal.Length() == 0.f)
            return;

        Normal.Normalize();

        // 벽과 날아가는 각도가 45도 이하일 경우 WallEnd상태로 이동
        if (Normal.Dot(-RayDir) >= cosf(XM_PI / 4.f))
        {
            ChangeState(L"BURNING_WALL_END");
        }
    }

}

void CKirbyBurning::Enter()
{
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);


    CGameObject* Wing = PLAYER->GetChildObject(L"KirbyDragon");

    if (Wing != nullptr)
    {
        Wing->SetActive(true);
    }

    Wing->Animator()->Play(ANIMPREFIX("Burning"), false, false, 1.5f);
    m_Acc = 0.f;

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("Burning"), true, false, 1.5f);

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

void CKirbyBurning::Exit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    CGameObject* Wing = PLAYER->GetChildObject(L"KirbyDragon");

    if (Wing != nullptr)
    {
        Wing->SetActive(false);
    }

    // Movement
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetForwardMode(false);
    PLAYERCTRL->SetRotSpeed(m_SaveRotSpeed);

    PLAYERCTRL->SetSpeed(m_SaveSpeed);

    //  무적 상태
    PLAYERFSM->SetInvincible(false);
}
