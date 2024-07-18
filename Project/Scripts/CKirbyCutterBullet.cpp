#include "pch.h"
#include "CKirbyCutterBullet.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CKirbyHatBlade.h"

CKirbyCutterBullet::CKirbyCutterBullet()
    : CScript(KIRBYCUTTERBULLET)
    , m_IsBack(false)
    , m_Speed(35.f)
    , m_RageSpeed(35.f)
    , m_Frequency(0.5f)
    , m_RotAcc(0.f)
    , m_Angle(45.f)
{
}

CKirbyCutterBullet::~CKirbyCutterBullet()
{
}

void CKirbyCutterBullet::begin()
{
    if (!Rigidbody())
    {
        GetOwner()->AddComponent(new CRigidbody());
    }

    // 애니메이션 재생
    Animator()->Play(KIRBYANIM(L"Spin"), false, false, 1.5f,0);

    // 초기 값 세팅
    m_IsBack = false;
    m_CurState = BulletState::DRAG;
    m_Velocity = Vec3(0.f, 0.f, 0.f);

    m_Duration = 0.4f;
    m_Acc = 0.f;

    m_MoveDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

    m_BounceCount = 6;


}

void CKirbyCutterBullet::tick()
{
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), m_MoveDir, 100.f, Vec3(1.f, 0.f, 0.f), true);

    // 좌우로 흔들리는 효과
    m_RotAcc += DT;

    float Angle = m_Angle * sinf((m_RotAcc / m_Frequency) * XM_PI * 2.f);

    Vec3 Front = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Front.Normalize();

    Vec3 AngularVelocity = Front * XMConvertToRadians(Angle);
    Rigidbody()->SetAngularVelocity(AngularVelocity);

    if (m_RotAcc > m_Frequency)
    {
        m_RotAcc -= m_Frequency;
    }

    if (Animator()->IsFinish())
    {
        Animator()->Play(KIRBYANIM(L"SpinBig"), true, false, 1.5f, 0.1);
    }

    // Bullter 움직임
    switch (m_CurState)
    {
    case BulletState::HOLD_AIR: {
        m_Acc += DT;

        if (m_Acc > m_Duration)
        {
            --m_BounceCount;

            if (m_IsBack)
            {
                GamePlayStatic::DestroyGameObject(GetOwner());
            }
            else
            {
                if (m_BounceCount == 0)
                {
                    GamePlayStatic::DestroyGameObject(GetOwner());
                }
                else
                {
                    SetState(BulletState::BACK);
                }
            }
        }
    }
    break;
    case BulletState::HOLD_WALL: {
        m_Acc += DT;

        Rigidbody()->SetVelocity({0.f, 0.f, 0.f});

        if (m_Acc > m_Duration)
        {
            --m_BounceCount;

            if (m_BounceCount == 0)
            {
                GamePlayStatic::DestroyGameObject(GetOwner());
            }
            else
            {
                if (m_IsBack)
                {
                    SetState(BulletState::RAGE);
                }
                else
                {
                    SetState(BulletState::DRAG);
                }
            }
        }
    }
    break;
    case BulletState::DRAG: {
        m_Acc += DT;

        float phase = (m_Acc / m_Duration) * XM_PI;
        phase = clamp(phase, 0.f, XM_PI);
        phase = std::sin(phase);

        m_Velocity = m_MoveDir * m_Speed * phase;

        Rigidbody()->SetVelocity(m_Velocity);

        if (m_Acc > m_Duration)
        {
            SetState(BulletState::HOLD_AIR);
        }
    }
    break;
    case BulletState::BACK: {
        m_Acc += DT;

        Vec3 KirbyPos = PLAYER->Transform()->GetWorldPos();
        KirbyPos += Vec3(0.f, 50.f, 0.f);
        Vec3 CurPos = Transform()->GetWorldPos();

        Vec3 NewDir = KirbyPos - CurPos;
        NewDir.Normalize();

        if (m_MoveDir.Dot(NewDir) > 0.f)
        {
            NewDir.y = m_MoveDir.y;
            m_MoveDir = DirectX::SimpleMath::Vector3::Lerp(m_MoveDir, NewDir, 0.15f);
            m_MoveDir.Normalize();
        }

        float phase = (m_Acc / 0.2f) * XM_PI / 2.f;
        phase = clamp(phase, 0.f, XM_PI/2.f);
        phase = std::sin(phase);

        m_Velocity = m_MoveDir * m_RageSpeed * phase;

        Rigidbody()->SetVelocity(m_Velocity);


        if (m_Acc > m_Duration)
        {
            GamePlayStatic::DestroyGameObject(GetOwner());
        }
    }
    break;
    case BulletState::RAGE: {
        m_Acc += DT;

        m_Velocity = m_MoveDir * m_RageSpeed;

        Rigidbody()->SetVelocity(m_Velocity);

        if (m_Acc > m_Duration)
        {
            GamePlayStatic::DestroyGameObject(GetOwner());
        }
    }
    break;
    }

}

void CKirbyCutterBullet::SetState(BulletState _State)
{
    m_CurState = _State;
    m_Acc = 0.f;

    switch (m_CurState)
    {
    case BulletState::HOLD_AIR: {
        m_Duration = 0.7f;
    }
    break;
    case BulletState::HOLD_WALL: {
        m_Duration = 0.7f;
    }
    break;
    case BulletState::DRAG: {
        m_Duration = 0.7f;
    }
    break;
    case BulletState::BACK: {
        // 방향 설정
        Vec3 KirbyPos = PLAYER->Transform()->GetWorldPos();
        Vec3 test = Transform()->GetWorldPos();

        Vec3 Dir = KirbyPos - Transform()->GetWorldPos() + Vec3(0.f,50.f,0.f);
        Dir.Normalize();

        m_MoveDir = Dir;

        m_Duration = 3.f;
        m_IsBack = true;
    }
    break;
    case BulletState::RAGE: {
        m_Duration = 3.f;
    }
    break;
    default:
        break;
    }
}


void CKirbyCutterBullet::OnTriggerEnter(CCollider* _OtherCollider)
{
    int LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();

    // WorldStatic
    if (LayerIdx == 2)
    {
        Vec3 CurVel = Rigidbody()->GetVelocity();
        Rigidbody()->SetVelocity({0.f,0.f,0.f});

        Vec3 RayDir = m_MoveDir;
        RayDir.y = 0.f;
        RayDir.Normalize();

        RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Transform()->GetWorldPos(), RayDir, 100.f, {L"World Static"});
        Vec3 Normal = Hit.Normal;

        m_MoveDir = RayDir + 2.f * Normal * (-RayDir.Dot(Normal));

        SetState(BulletState::HOLD_WALL);

    }
    // Player
    else if (LayerIdx == 4)
    {
        // 커비한테 돌아가는 상태라면
        if (m_IsBack)
        {
            GamePlayStatic::DestroyGameObject(GetOwner());


            if (PLAYERFSM->GetCurAbilityIdx() == AbilityCopyType::CUTTER && PLAYERFSM->GetCurObjectIdx() == ObjectCopyType::NONE)
            {
                CKirbyHatBlade* BladeScript = dynamic_cast<CKirbyHatBlade*>(PLAYERFSM->GetCurHatBlade()->GetScripts()[0]);

                if (BladeScript)
                {
                    BladeScript->Reset();
                }

            }

        }
    }


}

void CKirbyCutterBullet::OnTriggerExit(CCollider* _OtherCollider)
{

}

void CKirbyCutterBullet::SaveToLevelFile(FILE* _File)
{
}

void CKirbyCutterBullet::LoadFromLevelFile(FILE* _File)
{
}
