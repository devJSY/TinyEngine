#include "pch.h"
#include <Engine\\CScript.h>
#include "CElfilisLaser.h"

CElfilisLaser::CElfilisLaser()
    : CScript(ELFILISLASER)
    , m_CollisionParticle(nullptr)
    , m_Step(StateStep::Start)
    , m_bAutoPlay(false)
    , m_bCollisionFloor(false)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCollisionFloor, "Collision Floor");
    // m_MagmaPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisLaserMagma.pref");
    m_CollisionParticlePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Particle_ElfilisLaserCollision.pref");
}

CElfilisLaser::CElfilisLaser(const CElfilisLaser& _Origin)
    : CScript(_Origin)
    , m_CollisionParticle(nullptr)
    , m_Step(StateStep::Start)
    , m_bAutoPlay(_Origin.m_bAutoPlay)
    , m_bCollisionFloor(_Origin.m_bCollisionFloor)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCollisionFloor, "Collision Floor");
    // m_MagmaPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisLaserMagma.pref");
    m_CollisionParticlePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Particle_ElfilisLaserCollision.pref");
}

CElfilisLaser::~CElfilisLaser()
{
    if (m_CollisionParticle)
    {
        GamePlayStatic::DestroyGameObject(m_CollisionParticle);
        m_CollisionParticle = nullptr;
    }
}

void CElfilisLaser::begin()
{
    if (m_bAutoPlay)
    {
        SetStart();
    }

    CGameObject* Outer = GetOwner()->GetChildObject()[0];
    UINT Layer = GetOwner()->GetLayerIdx();

    if (Outer && Outer->GetLayerIdx() != Layer)
    {
        GamePlayStatic::LayerChange(Outer, Layer);
    }
}

void CElfilisLaser::tick()
{
    if (!m_bAutoPlay)
        return;

    switch (m_Step)
    {
    case StateStep::Start:
        Start();
        break;
    case StateStep::Progress:
        Progress();
        break;
    case StateStep::End:
        End();
        break;
    }
}

void CElfilisLaser::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (!m_bCollisionFloor || _OtherCollider->GetOwner()->GetName() != L"Floor")
        return;

    static vector<wstring> vecCollision{L"World Static"};
    RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), 2000.f, vecCollision);

    if (Hit.pCollisionObj)
    {
        if (m_CollisionParticlePref != nullptr)
        {
            if (!m_CollisionParticle)
            {
                m_CollisionParticle = m_CollisionParticlePref->Instantiate();
                GamePlayStatic::SpawnGameObject(m_CollisionParticle, LAYER_EFFECT);
            }

            m_CollisionParticle->Transform()->SetWorldPos(Hit.Point);
        }
    }
}

void CElfilisLaser::OnTriggerStay(CCollider* _OtherCollider)
{
    if (!m_bCollisionFloor || _OtherCollider->GetOwner()->GetName() != L"Floor")
        return;

    static vector<wstring> vecCollision{L"World Static"};
    RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), 2000.f, vecCollision);

    if (Hit.pCollisionObj)
    {
        if (m_CollisionParticle)
        {
            m_CollisionParticle->Transform()->SetWorldPos(Hit.Point);
        }

        //@EFFECT magma ¼ÒÈ¯
    }
}

void CElfilisLaser::OnTriggerExit(CCollider* _OtherCollider)
{
    if (!m_bCollisionFloor || _OtherCollider->GetOwner()->GetName() != L"Floor")
        return;

    static vector<wstring> vecCollision{L"World Static"};
    RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), 2000.f, vecCollision);

    if (!Hit.pCollisionObj)
    {
        if (m_CollisionParticle)
        {
            GamePlayStatic::DestroyGameObject(m_CollisionParticle);
            m_CollisionParticle = nullptr;
        }
    }
}

void CElfilisLaser::SetStart()
{
    m_Step = StateStep::Start;
    Animator()->Play(ANIMPREFIX("Start"), false);
    PlayOuter(ANIMPREFIX("Start"), false);
}

void CElfilisLaser::SetWait()
{
    m_Step = StateStep::Progress;
    Animator()->Play(ANIMPREFIX("Loop"));
    PlayOuter(ANIMPREFIX("Loop"));

    // sound
    wstring SoundKeyLaser;
    int RandIdx = GetRandomInt(0, 1);

    if (RandIdx == 0)
        SoundKeyLaser = L"sound\\wav\\CharaBossChimeraSoul\\0015_DimensionLaser_Laser.wav";
    else
        SoundKeyLaser = L"sound\\wav\\CharaBossChimeraSoul\\0016_DimensionLAser_Laser.wav";

    GamePlayStatic::Play2DSound(SoundKeyLaser, 1, SOUND_ELFILIS * 0.6f, true, false);
}

void CElfilisLaser::SetEnd()
{
    m_Step = StateStep::End;
    Animator()->Play(ANIMPREFIX("End"), false);
    PlayOuter(ANIMPREFIX("End"), false);

    if (m_CollisionParticle)
    {
        GamePlayStatic::DestroyGameObject(m_CollisionParticle);
        m_CollisionParticle = nullptr;
    }
}

void CElfilisLaser::SafeDestroy()
{
    if (m_CollisionParticle)
    {
        GamePlayStatic::DestroyGameObject(m_CollisionParticle);
        m_CollisionParticle = nullptr;
    }

    GamePlayStatic::DestroyGameObject(GetOwner());
}

void CElfilisLaser::Start()
{
    if (Animator()->IsFinish())
    {
        SetWait();
    }
}

void CElfilisLaser::Progress()
{
}

void CElfilisLaser::End()
{
    if (Animator()->IsFinish())
    {
        GamePlayStatic::DestroyGameObject(GetOwner());

        if (m_CollisionParticle)
        {
            GamePlayStatic::DestroyGameObject(m_CollisionParticle);
            m_CollisionParticle = nullptr;
        }
    }
}

void CElfilisLaser::PlayOuter(const wstring _Anim, bool _bRepeat)
{
    CGameObject* Outer = GetOwner()->GetChildObject()[0];

    if (!Outer)
        return;

    Outer->Animator()->Play(_Anim, _bRepeat);
}

UINT CElfilisLaser::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bCollisionFloor, sizeof(bool), 1, _File);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CElfilisLaser::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bCollisionFloor, sizeof(bool), 1, _File);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}