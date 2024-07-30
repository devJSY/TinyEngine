#include "pch.h"
#include "CKirbyCutterBullet.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CKirbyHatBlade.h"
#include "CMonsterUnitScript.h"

CKirbyCutterBullet::CKirbyCutterBullet()
    : CScript(KIRBYCUTTERBULLET)
    , m_IsBack(false)
    , m_Speed(35.f)
    , m_BurningSpeed(40.f)
    , m_Frequency(0.7f)
    , m_RotAcc(0.f)
    , m_Angle(45.f)
    , m_bPressedATK(true)
    , m_bBuning(false)
    , m_BuningStepUpDuration(1.f)
    , m_CurStep(BuningStep::NONE)
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
    Animator()->Play(ANIMPREFIX("Spin"), false, false, 1.5f, 0);

    // 초기 값 세팅
    m_IsBack = false;
    m_CurState = BulletState::DRAG;
    m_Velocity = Vec3(0.f, 0.f, 0.f);

    m_Duration = 0.4f;
    m_Acc = 0.f;

    m_MoveDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

    m_BounceCount = 6;
    m_bPressedATK = true;
    m_bBuning = false;
    m_BuningStepUpDuration = 1.f;
    m_CurStep = BuningStep::NONE;
}

void CKirbyCutterBullet::tick()
{
    switch (m_CurState)
    {
    case BulletState::HOLD_AIR:
        GetOwner()->SetName(L"AIR");
        break;
    case BulletState::HOLD_WALL:
        GetOwner()->SetName(L"WALL");
        break;
    case BulletState::DRAG:
        GetOwner()->SetName(L"DRAG");
        break;
    case BulletState::BACK:
        GetOwner()->SetName(L"BACK");
        break;
    case BulletState::RAGE:
        GetOwner()->SetName(L"RAGE");
        break;
    case BulletState::BURNING_WAIT:
        GetOwner()->SetName(L"BURNING_WAIT");
        break;
    case BulletState::BURNING:
        GetOwner()->SetName(L"BURNING");
        break;
    default:
        break;
    }

    // Burning check
    if (m_bPressedATK && (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK)))
    {
        m_bPressedATK = false;
    }

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
        Animator()->Play(ANIMPREFIX("SpinBig"), true, false, 1.5f, 0.1);
    }

    // Bullter 움직임
    switch (m_CurState)
    {
    case BulletState::HOLD_AIR: {
        m_Acc += DT;

        if (m_bPressedATK)
        {
            SetState(BulletState::BURNING_WAIT);
        }
        else if (m_Acc > m_Duration)
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
                if (m_bBuning)
                {
                    SetState(BulletState::BURNING);
                }
                else if (m_IsBack)
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
        phase = clamp(phase, 0.f, XM_PI / 2.f);
        phase = std::sin(phase);

        m_Velocity = m_MoveDir * m_Speed * phase;

        Rigidbody()->SetVelocity(m_Velocity);

        if (m_Acc > m_Duration)
        {
            GamePlayStatic::DestroyGameObject(GetOwner());
        }
    }
    break;
    case BulletState::RAGE: {
        m_Acc += DT;

        float phase = (m_Acc / 0.2f) * XM_PI / 2.f;
        phase = clamp(phase, 0.f, XM_PI / 2.f);
        phase = std::sin(phase);

        m_Velocity = m_MoveDir * m_Speed * phase;

        Rigidbody()->SetVelocity(m_Velocity);

        if (m_Acc > m_Duration)
        {
            GamePlayStatic::DestroyGameObject(GetOwner());
        }
    }
    break;
    case BulletState::BURNING_WAIT: {
        m_Acc += DT;

        // @TODO SCALA_PARAM을 이용하여 색이랑 크기가 시간에 따라 변하도록 해야함

        if (m_Acc > m_BuningStepUpDuration)
        {
            // StepUP 했을 경우
            if (StepUp())
            {
                m_Acc -= m_BuningStepUpDuration;
            }
        }

        if (m_Acc > m_Duration || KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
        {
            SetState(BulletState::BACK);
        }
    }
    break;
    case BulletState::BURNING: {
        m_Acc += DT;

        float phase = (m_Acc / 0.2f) * XM_PI / 2.f;
        phase = clamp(phase, 0.f, XM_PI / 2.f);
        phase = std::sin(phase);

        m_Velocity = m_MoveDir * m_BurningSpeed * phase;

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

        Vec3 Dir = KirbyPos - Transform()->GetWorldPos() + Vec3(0.f, 20.f, 0.f);
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
    case BulletState::BURNING_WAIT: {
        // @TODO 다이나믹 머터리얼로 머터리얼을 교체한뒤 쉐이더를 전용쉐이더로 바꿔줘야함
        MeshRender()->GetDynamicMaterial(0);

        m_Duration = 4.f;
    }
    break;
    case BulletState::BURNING: {
        m_Duration = 3.f;
        m_bBuning = true;
    }
    break;

    default:
        break;
    }
}

bool CKirbyCutterBullet::StepUp()
{

    switch (m_CurStep)
    {
    case BuningStep::NONE: {
        m_CurStep = BuningStep::STEP_1;

        Ptr<CMaterial> DynamicMtrl = MeshRender()->GetMaterial(0);
        DynamicMtrl->SetEmission(Vec4(0.3f, 0.3f, 0.3f, 0.3f));
    }
    break;
    case BuningStep::STEP_1: {
        m_CurStep = BuningStep::STEP_2;

        Ptr<CMaterial> DynamicMtrl = MeshRender()->GetMaterial(0);
        DynamicMtrl->SetEmission(Vec4(0.6f, 0.6f, 0.6f, 0.6f));
    }
    break;
    case BuningStep::STEP_2: {
        m_CurStep = BuningStep::STEP_3;

        Ptr<CMaterial> DynamicMtrl = MeshRender()->GetMaterial(0);
        DynamicMtrl->SetEmission(Vec4(0.9f, 0.9f, 0.9f, 0.9f));
    }
    break;
    case BuningStep::STEP_3: {
        return false;
    }
    break;
    }

    return true;
}

void CKirbyCutterBullet::OnCollisionEnter(CCollider* _OtherCollider)
{
    int LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();

    // WorldStatic
    if (LayerIdx == LAYER_STATIC)
    {
        Vec3 RayDir = m_MoveDir;
        RayDir.y = 0.f;
        RayDir.Normalize();

        Vec3 RayStart = Transform()->GetWorldPos() - m_MoveDir * 10.f;

        RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(RayStart, RayDir, 1000.f, {L"World Static"});

        if (Hit.pCollisionObj != nullptr)
        {
            Vec3 Normal = Hit.Normal;
            m_MoveDir = RayDir + 2.f * Normal * (-RayDir.Dot(Normal));
        }
        else
        {
            m_MoveDir *= -1.f;
        }

        SetState(BulletState::HOLD_WALL);
    }

}

void CKirbyCutterBullet::OnTriggerEnter(CCollider* _OtherCollider)
{
    int LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();

    // WorldStatic
    if (LayerIdx == LAYER_PLAYER_TRIGGER)
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
    // Monster
    else if (LayerIdx == LAYER_MONSTER_TRIGGER)
    {
        CMonsterUnitScript* Monster = _OtherCollider->GetOwner()->GetScript<CMonsterUnitScript>();
        if (nullptr != Monster)
        {
            Vec3 HitDir = _OtherCollider->Transform()->GetWorldPos() - Transform()->GetWorldPos();
            HitDir.Normalize();
            UnitHit HitInfo = {DAMAGE_TYPE::NORMAL, HitDir, 10.f, 0.f, 0.f};

            Monster->GetDamage(HitInfo);
        }
    }
}

void CKirbyCutterBullet::OnTriggerExit(CCollider* _OtherCollider)
{
}

UINT CKirbyCutterBullet::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CKirbyCutterBullet::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}
