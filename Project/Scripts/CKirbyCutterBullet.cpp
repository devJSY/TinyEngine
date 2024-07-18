#include "pch.h"
#include "CKirbyCutterBullet.h"

#include "CPlayerMgr.h"

CKirbyCutterBullet::CKirbyCutterBullet()
    : CScript(KIRBYCUTTERBULLET)
    , m_IsBack(false)
    , m_Speed(35.f)
    , m_RageSpeed(40.f)
    , m_Frequency(1.f)
    , m_RotAcc(0.f)
    , m_Angle(30.f)
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
    Animator()->Play(KIRBYANIM(L"Spin"), true, false, 1.5f,0);

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
    // 좌우로 흔들리는 효과
    m_RotAcc += DT;

    float Angle = 30.f * sinf((m_RotAcc / m_Frequency) * XM_PI * 2.f);

    Vec3 Front = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Front.Normalize();

    Vec3 AngularVelocity = Front * XMConvertToRadians(Angle);
    Rigidbody()->SetAngularVelocity(AngularVelocity);

    if (m_RotAcc > m_Frequency)
    {
        m_RotAcc -= m_Frequency;
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

        m_Velocity = m_MoveDir * m_RageSpeed;

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
        Vec3 Dir = KirbyPos - Transform()->GetWorldPos();
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

void CKirbyCutterBullet::OnCollisionEnter(CCollider* _OtherCollider)
{
    int a = 0;
}

void CKirbyCutterBullet::OnCollisionStay(CCollider* _OtherCollider)
{
    int a = 0;
}

void CKirbyCutterBullet::OnCollisionExit(CCollider* _OtherCollider)
{
    int a = 0;
}

void CKirbyCutterBullet::OnTriggerEnter(CCollider* _OtherCollider)
{
    int a = 0;
}

void CKirbyCutterBullet::OnTriggerStay(CCollider* _OtherCollider)
{
    int a = 0;
}

void CKirbyCutterBullet::OnTriggerExit(CCollider* _OtherCollider)
{
    int a = 0;
}

void CKirbyCutterBullet::SaveToLevelFile(FILE* _File)
{
}

void CKirbyCutterBullet::LoadFromLevelFile(FILE* _File)
{
}
