#include "pch.h"
#include "CMorphoUnit.h"
#include "CPlayerMgr.h"
#include "CCameraController.h"
#include "CBossMgr.h"
#include "CMorphoFSM.h"
#include "CBossLevelFlowMgr.h"
#include "CDestroyParticleScript.h"
#include "CChangeAlphaScript.h"

CMorphoUnit::CMorphoUnit()
    : CUnitScript(MORPHOUNIT)
{
    UnitInfo MorphoInfo = {
        800.f, // HP
        800.f, // MaxHP
        10.f,  // Speed
        10.f,  // Rotation Speed
        10.f,  // JumpPower
        0.f,   // ATK
    };
    SetInitInfo(MorphoInfo);

    m_LightningEffectPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_MorphoLightningSet.pref", L"prefab\\Effect_MorphoLightningSet.pref");
}

CMorphoUnit::CMorphoUnit(const CMorphoUnit& _Origin)
    : CUnitScript(_Origin)
{
    UnitInfo MorphoInfo = {
        800.f, // HP
        800.f, // MaxHP
        10.f,  // Speed
        10.f,  // Rotation Speed
        10.f,  // JumpPower
        0.f,   // ATK
    };
    SetInitInfo(MorphoInfo);

    m_LightningEffectPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_MorphoLightningSet.pref", L"prefab\\Effect_MorphoLightningSet.pref");
}

CMorphoUnit::~CMorphoUnit()
{
}

void CMorphoUnit::tick()
{
    CUnitScript::tick();

    // Death
    if (m_CurInfo.HP <= 0.f && CBossMgr::GetBossFlowMgr()->GetFlowState() < BossLevelFlow::Death)
    {
        MRPFSM->ResetFSM();
        MRPFSM->ChangeStateGroup(MorphoStateGroup::DEMO, L"DEMO_DEATH");
    }

    // Phase 1
    else if (MRPFSM->GetPhase() == 1)
    {
        // Cheat : Phase 1 -> Phase 2
        if ((KEY_PRESSED(KEY::LCTRL) && (KEY_TAP(KEY::ENTER))) || (KEY_TAP(KEY::LCTRL) && (KEY_PRESSED(KEY::ENTER))))
        {
            m_CurInfo.HP = m_InitInfo.HP * 0.5f;
        }

        if (m_CurInfo.HP <= m_InitInfo.HP * 0.5f)
        {
            MRPFSM->ResetFSM();
            MRPFSM->SetPhase(2);
            MRPFSM->ChangeStateGroup(MorphoStateGroup::DEMO, L"DEMO_PHASE2");
        }
    }

    // Phase 2
    else if (MRPFSM->GetPhase() == 2)
    {
        // Cheat : Death
        if ((KEY_PRESSED(KEY::LCTRL) && (KEY_TAP(KEY::ENTER))) || (KEY_TAP(KEY::LCTRL) && (KEY_PRESSED(KEY::ENTER))))
        {
            m_CurInfo.HP = 0.f;
        }
    }
}

CGameObject* CMorphoUnit::SpawnAttackButterflyEffect(Vec3 _Pos)
{
    CGameObject* Butterfly = new CGameObject;
    CParticleSystem* Particle = nullptr;
    int Rand = GetRandomInt(0, 2);

    if (Rand == 0)
    {
        Particle = MRPFSM->GetParticleButterflyPink()->ParticleSystem()->Clone();
    }
    else if (Rand == 1)
    {
        Particle = MRPFSM->GetParticleButterflyYellow()->ParticleSystem()->Clone();
    }
    else if (Rand == 2)
    {
        Particle = MRPFSM->GetParticleButterflyYellowPink()->ParticleSystem()->Clone();
    }

    if (Particle)
    {
        tParticleModule Module = Particle->GetParticleModule();
        Module.vSpawnMinScale = Vec3(20.f, 20.f, 1.f);
        Module.vSpawnMaxScale = Vec3(40.f, 40.f, 1.f);
        Module.vScaleRatio = Vec3::Zero;
        Module.MinSpeed = 30.f;
        Module.MaxSpeed = 80.f;
        Module.VelocityAlignment = GetRandomInt(0, 1);
        Module.AlphaBasedLife = 1;

        Particle->SetParticleModule(Module);
        Butterfly->AddComponent(Particle);
    }

    _Pos.y = GetRandomfloat(0.f, 50.f);
    Butterfly->AddComponent(new CTransform);
    Butterfly->Transform()->SetWorldPos(_Pos);

    CDestroyParticleScript* Script = new CDestroyParticleScript;
    Script->SetSpawnTime(0.5f);
    Butterfly->AddComponent(Script);

    Butterfly->SetName(L"Particle_Butterfly");
    GamePlayStatic::SpawnGameObject(Butterfly, LAYER_EFFECT);

    return Butterfly;
}

CGameObject* CMorphoUnit::SpawnLightningEffect(Vec3 _Pos)
{
    if (m_LightningEffectPref == nullptr)
        return nullptr;

    CGameObject* Lightning = m_LightningEffectPref->Instantiate();
    Vec3 Dir = CPlayerMgr::GetCameraController()->GetLookDir() * -1.f;
    Dir.y = 0.f;

    Lightning->Transform()->SetWorldPos(_Pos);
    Lightning->Transform()->Slerp(Dir.Normalize(), 1.f);

    CChangeAlphaScript* Script = Lightning->GetScript<CChangeAlphaScript>();
    Script->FadeIn_RandomDelay(0.f, 0.4f);

    GamePlayStatic::SpawnGameObject(Lightning, LAYER_EFFECT);

    return Lightning;
}

CGameObject* CMorphoUnit::SpawnCircleDustEffect(Vec3 _Pos)
{
    CGameObject* Dust = new CGameObject;
    CParticleSystem* Particle = MRPFSM->GetParticleCircleDust()->ParticleSystem()->Clone();
    Dust->AddComponent(Particle);

    _Pos.y = GetRandomfloat(0.f, 10.f);
    Dust->AddComponent(new CTransform);
    Dust->Transform()->SetWorldPos(_Pos);

    CDestroyParticleScript* Script = new CDestroyParticleScript;
    Script->SetSpawnTime(0.5f);
    Dust->AddComponent(Script);

    Dust->SetName(L"Particle_CircleDust");
    GamePlayStatic::SpawnGameObject(Dust, LAYER_EFFECT);

    return Dust;
}

UINT CMorphoUnit::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    //MemoryByte += CUnitScript::SaveToLevelFile(_File);
    
    return MemoryByte;
}

UINT CMorphoUnit::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    // MemoryByte += CUnitScript::LoadFromLevelFile(_File);

    return MemoryByte;
}