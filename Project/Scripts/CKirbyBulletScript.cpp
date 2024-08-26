#include "pch.h"
#include "CKirbyBulletScript.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CUnitScript.h"
#include "CKirbyBulletSize.h"

CKirbyBulletScript::CKirbyBulletScript()
    : CScript(KIRBYBULLETSCRIPT)
    , m_ScaleFactor(2.f / 3.f)
    , m_Speed(1.f)
    , m_PlayTime(5.f)
    , m_bHasTickCol(false)
    , m_bInit(true)
{
}

CKirbyBulletScript::~CKirbyBulletScript()
{
}

void CKirbyBulletScript::begin()
{
    if (!Rigidbody())
    {
        GetOwner()->AddComponent(new CRigidbody());
    }

    Rigidbody()->SetUseGravity(false);

    // ===========================
    // Create Child
    // ===========================
    if (!PLAYERFSM->GetStuffedObj())
        return;

    CGameObject* pStuffedObj = PLAYERFSM->GetStuffedObj()->Clone();
    pStuffedObj->SetActive(true);

    // set SclaeFactor
    CKirbyBulletSize* pSizeScript = PLAYERFSM->GetStuffedObj()->GetScript<CKirbyBulletSize>();

    if (pSizeScript)
    {
        int Size = pSizeScript->GetSizeType(); // SizeType : 1~3

        if (Size == 2)
        {
            m_ScaleFactor = 0.4f;
        }
        else if (Size == 3)
        {
            m_ScaleFactor = 0.2f;
        }
    }

    // set offset
    if (PLAYERFSM->GetStuffedObj()->SphereCollider())
    {
        m_ChildOffest = PLAYERFSM->GetStuffedObj()->SphereCollider()->GetCenter();
    }
    else if (PLAYERFSM->GetStuffedObj()->CapsuleCollider())
    {
        m_ChildOffest = PLAYERFSM->GetStuffedObj()->CapsuleCollider()->GetCenter();
    }
    else if (PLAYERFSM->GetStuffedObj()->BoxCollider())
    {
        m_ChildOffest = PLAYERFSM->GetStuffedObj()->BoxCollider()->GetCenter();
    }
    else if (PLAYERFSM->GetStuffedObj()->MeshCollider())
    {
        m_ChildOffest = PLAYERFSM->GetStuffedObj()->MeshCollider()->GetCenter();
    }

    // add component
    pStuffedObj->Transform()->SetAbsolute(false);
    pStuffedObj->Transform()->SetLocalRotation(Vec3());
    pStuffedObj->Transform()->SetLocalScale(Vec3(m_ScaleFactor));

    if (PLAYERFSM->GetStuffedObj()->Animator())
    {
        pStuffedObj->Animator()->Play(ANIMPREFIX("Damage"), true, false, 1.f);
    }

    pStuffedObj->SetName(L"StuffedObjCopy");
    GamePlayStatic::AddChildObject(GetOwner(), pStuffedObj);
    GamePlayStatic::LayerChange(pStuffedObj, LAYER_PLAYERATK);
}

void CKirbyBulletScript::tick()
{
    // init child pos
    if (m_bInit && GetOwner()->GetChildObject(L"StuffedObjCopy"))
    {
        GetOwner()->GetChildObject(L"StuffedObjCopy")->Transform()->SetLocalPos(-m_ChildOffest * m_ScaleFactor);
        m_bInit = false;
    }

    m_bHasTickCol = false;
    m_PlayTime -= DT;
    Rigidbody()->SetVelocity(m_MoveDir * m_Speed);
    Vec3 AngularVelocity = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Rigidbody()->SetAngularVelocity(AngularVelocity);

    if (m_PlayTime <= 0.f)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CKirbyBulletScript::SetInitVelocity(Vec3 _Vel)
{
    m_Speed = _Vel.Length();

    _Vel.y = 0.f;
    _Vel = _Vel.Normalize();
    m_MoveDir = _Vel;
}

void CKirbyBulletScript::OnCollisionEnter(CCollider* _OtherCollider)
{
    int LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();

    // world static : bullet 튕김 구현
    if (LayerIdx == LAYER_STATIC)
    {
        if (m_bHasTickCol)
            return;

        Vec3 ColRadius = SphereCollider()->GetRadius() * Transform()->GetWorldScale();
        Vec3 RayStart = Transform()->GetWorldPos() + m_MoveDir * ColRadius;
        RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(RayStart, m_MoveDir, ColRadius.Length() + 10.f, {L"World Static"});

        if (Hit.pCollisionObj)
        {
            m_bHasTickCol = true;
            Vec3 Normal = Hit.Normal;
            m_MoveDir = m_MoveDir + 2.f * Normal * (-m_MoveDir.Dot(Normal));
            m_MoveDir.Normalize();

            // 예외처리 Dir 이 Vec3(0.f, 0.f, -1.f)인경우 Up벡터가 반전됨
            Vec3 up = Vec3(0.f, 1.f, 0.f);
            if (m_MoveDir == Vec3(0.f, 0.f, -1.f))
            {
                up = Vec3(0.f, -1.f, 0.f);
            }

            Quat ToWardQuaternion = Quat::LookRotation(-m_MoveDir, up);
            Transform()->SetWorldRotation(ToWardQuaternion);
        }
    }

    // monster : 데미지 가함
    else if (LayerIdx == LAYER_MONSTER)
    {
        CUnitScript* pMonster = _OtherCollider->GetOwner()->GetScript<CUnitScript>();
        if (!pMonster)
            return;

        Vec3 HitDir = (_OtherCollider->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
        UnitHit HitInfo = {DAMAGE_TYPE::NORMAL, HitDir, 20.f, 0.f, 0.f};

        pMonster->GetDamage(HitInfo);
        ((CUnitScript*)PLAYERUNIT)->AttackReward();
    }
}

void CKirbyBulletScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    int LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();

    if (LayerIdx == LAYER_MONSTER_TRIGGER)
    {
        if (_OtherCollider->GetOwner()->GetName() == L"Body Collider")
        {
            CUnitScript* pMonster = _OtherCollider->GetOwner()->GetParent()->GetScript<CUnitScript>();
            if (!pMonster)
                return;

            Vec3 HitDir = (_OtherCollider->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
            UnitHit HitInfo = {DAMAGE_TYPE::NORMAL, HitDir, 20.f, 0.f, 0.f};
            pMonster->GetDamage(HitInfo);
            ((CUnitScript*)PLAYERUNIT)->AttackReward();
        }
    }
}

UINT CKirbyBulletScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CKirbyBulletScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}
