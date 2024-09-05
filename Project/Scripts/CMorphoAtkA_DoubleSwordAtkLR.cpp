#include "pch.h"
#include "CMorphoAtkA_DoubleSwordAtkLR.h"
#include "CMorphoFSM.h"
#include "CPlayerMgr.h"
#include "CCameraController.h"
#include "CChangeAlphaScript.h"
#include "CDestroyParticleScript.h"

CMorphoAtkA_DoubleSwordAtkLR::CMorphoAtkA_DoubleSwordAtkLR()
    : m_LightningEffect{nullptr,}
    , m_FireSwipe{nullptr,}
    , m_bFrmEnter(true)
    , m_bOverlapBurning(false)
{
    m_FireSwipePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\MorphoFireSwipe.pref", L"prefab\\MorphoFireSwipe.pref");
}

CMorphoAtkA_DoubleSwordAtkLR::~CMorphoAtkA_DoubleSwordAtkLR()
{
}

void CMorphoAtkA_DoubleSwordAtkLR::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Progress: {
        Progress();
    }
    break;
    case StateStep::End: {
        End();
    }
    break;
    case StateStep::EndWait: {
        EndWait();
    }
    break;
    case StateStep::EndEnd: {
        EndEnd();
    }
    break;
    }
}

void CMorphoAtkA_DoubleSwordAtkLR::Exit()
{
    Exit_Step();

    if (m_Step < StateStep::EndEnd)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (m_LightningEffect[i])
            {
                GamePlayStatic::DestroyGameObject(m_LightningEffect[i]);
                m_LightningEffect[i] = nullptr;
            }
        }

        for (int i = 0; i < 2; ++i)
        {
            if (m_FireSwipe[i])
            {
                GamePlayStatic::DestroyGameObject(m_FireSwipe[i]);
                m_FireSwipe[i] = nullptr;
            }
        }

        MRPFSM->GetWeaponL()->SetActive(false);
    }

    MRPFSM->OffWeaponLTrigger();
    MRPFSM->OffWeaponRTrigger();

    // sound
    wstring Sound = L"sound\\wav\\CharaMorphoknight\\0034_FireNoise.wav";
    GamePlayStatic::StopSound(Sound);
}

void CMorphoAtkA_DoubleSwordAtkLR::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack3Start"), false, false, 1.5f);
        MRPFSM->OnWeaponLTrigger();
        MRPFSM->OnWeaponRTrigger();
        //@EFFECT 차징이펙트

        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\DoubleSwordCharging.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 0.9f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack3"), false, false, 1.5f);
        m_bFrmEnter = true;

        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\DoubleSwordAttackStart.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 0.8f);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack3End"), false, false, 1.5f);
        m_bFrmEnter = true;

        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\DoubleSwordAttackStart.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 0.4f);
    }
    break;
    case StateStep::EndWait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordWait"), true, false, 1.5f);
        m_bFrmEnter = true;
        m_SwipeTargetDir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    }
    break;
    case StateStep::EndEnd: {
        m_AccTime = 0.f;
    }
    break;
    }
}

void CMorphoAtkA_DoubleSwordAtkLR::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        // sound
        wstring SoundCharging = L"sound\\wav\\CharaMorphoknight\\DoubleSwordCharging.wav";
        CSoundMgr::GetInst()->FadeSound(SoundCharging, SOUND_MORPHO * 0.9f, 0.f, 0.5f, false);
    }
        break;
    case StateStep::Progress: {
        for (int i = 0; i < 2; ++i)
        {
            if (m_FireSwipe[i])
            {
                m_FireSwipe[i]->Transform()->SetLocalScale(m_SwipeOriginScale);
            }
        }
    }
    break;
    case StateStep::End:
        break;
    case StateStep::EndWait: {
        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\0034_FireNoise.wav";
        GamePlayStatic::StopSound(Sound);
    }
    break;
    case StateStep::EndEnd: {
        // Spawn DropStar
        if (m_FireSwipe)
        {
            Vec3 SpawnPos = m_FireSwipe[0]->Transform()->GetWorldPos();
            Vec3 FrontDir = m_FireSwipe[0]->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            Vec3 RightDir = m_FireSwipe[0]->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
            FrontDir.y = RightDir.y = 0.f;
            FrontDir.y = 0.f;
            FrontDir.Normalize();
            RightDir.Normalize();

            for (int i = 0; i < 3; ++i)
            {
                Vec3 OffsetSpawnPos = SpawnPos;
                OffsetSpawnPos += RightDir * 20.f;
                OffsetSpawnPos += FrontDir * (50.f + 100.f * i);

                MRPFSM->SpawnDropStar(OffsetSpawnPos);
            }

            for (int i = 0; i < 3; ++i)
            {
                Vec3 OffsetSpawnPos = SpawnPos;
                OffsetSpawnPos -= RightDir * 20.f;
                OffsetSpawnPos += FrontDir * (100.f + 100.f * i);

                MRPFSM->SpawnDropStar(OffsetSpawnPos);
            }
        }

        // delete
        for (int i = 0; i < 2; ++i)
        {
            if (m_FireSwipe[i])
            {
                GamePlayStatic::DestroyGameObject(m_FireSwipe[i]);
                m_FireSwipe[i] = nullptr;
            }
        }

        MRPFSM->GetWeaponL()->SetActive(false);
    }
    break;
    }
}

void CMorphoAtkA_DoubleSwordAtkLR::Start()
{
    RotateToPlayer(DT);

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoAtkA_DoubleSwordAtkLR::Progress()
{
    int SpawnFrm = 20;

    // Spawn Effect
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), SpawnFrm))
    {
        m_bFrmEnter = false;

        // spawn lightning & dust
        // Left
        Vec3 Pos = MRPFSM->GetWeaponL()->GetChildObject()[0]->Transform()->GetWorldPos();
        Pos.y = 0.f;
        m_LightningEffect[0] = MRPFSM->GetUnit()->SpawnLightningEffect(Pos);
        MRPFSM->GetUnit()->SpawnCircleDustEffect(Pos);

        // Right
        Pos = MRPFSM->GetWeaponR()->GetChildObject()[0]->Transform()->GetWorldPos();
        Pos.y = 0.f;
        m_LightningEffect[1] = MRPFSM->GetUnit()->SpawnLightningEffect(Pos);
        MRPFSM->GetUnit()->SpawnCircleDustEffect(Pos);

        // Spawn Particle Butterfly
        Vec3 SpawnPos = MRPFSM->GetWeaponR()->Transform()->GetWorldPos();
        Vec3 FrontDir = MRPFSM->GetWeaponR()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
        FrontDir.y = 0.f;
        FrontDir.Normalize();

        for (int i = 0; i < 6; ++i)
        {
            Vec3 OffsetSpawnPos = SpawnPos;
            OffsetSpawnPos += FrontDir * (50.f * i);
            MRPFSM->GetUnit()->SpawnAttackButterflyEffect(OffsetSpawnPos);
        }

        SpawnPos = MRPFSM->GetWeaponL()->Transform()->GetWorldPos();
        FrontDir = MRPFSM->GetWeaponL()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
        FrontDir.y = 0.f;
        FrontDir.Normalize();

        for (int i = 0; i < 6; ++i)
        {
            Vec3 OffsetSpawnPos = SpawnPos;
            OffsetSpawnPos += FrontDir * (50.f * i);
            MRPFSM->GetUnit()->SpawnAttackButterflyEffect(OffsetSpawnPos);
        }

        // Spawn FireSwipe
        if (m_FireSwipePref != nullptr)
        {
            Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
            Pos.y = 0.f;

            // Left
            m_FireSwipe[0] = m_FireSwipePref->Instantiate();
            Vec3 Dir = MRPFSM->GetWeaponL()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            Dir.y = 0.f;
            Dir.Normalize();

            m_FireSwipe[0]->Transform()->SetWorldPos(Pos);
            m_FireSwipe[0]->Transform()->Slerp(Dir.Normalize(), 1.f);

            GamePlayStatic::SpawnGameObject(m_FireSwipe[0], LAYER_MONSTERATK_TRIGGER);

            // Right
            m_FireSwipe[1] = m_FireSwipePref->Instantiate();
            Dir = MRPFSM->GetWeaponR()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            Dir.y = 0.f;
            Dir.Normalize();

            m_FireSwipe[1]->Transform()->SetWorldPos(Pos);
            m_FireSwipe[1]->Transform()->Slerp(Dir.Normalize(), 1.f);

            m_SwipeOriginScale = m_FireSwipe[0]->Transform()->GetLocalScale();
            GamePlayStatic::SpawnGameObject(m_FireSwipe[1], LAYER_MONSTERATK_TRIGGER);

            // sound
            wstring Sound = L"sound\\wav\\CharaMorphoknight\\0034_FireNoise.wav";
            GamePlayStatic::Play2DSound(Sound, 0, SOUND_MORPHO * 0.8f);
        }

        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\DoubleSwordAttackCollision.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 1.5f);

        CAMERACTRL->Shake(0.6f, 30.f, 55.f);
    }

    // Spawn FireSwipe
    if (CHECK_ANIMFRM(GetOwner(), SpawnFrm))
    {
        for (int i = 0; i < 2; ++i)
        {
            if (m_FireSwipe[i])
            {
                int ClipIdx = GetOwner()->Animator()->GetCurClipIdx();
                int FrmLen = GetOwner()->Animator()->GetSkeletalMesh()->GetAnimClip()->at(ClipIdx).iFrameLength;
                float t = float(GetOwner()->Animator()->GetClipFrameIndex()) / float(FrmLen - SpawnFrm);
                Vec3 NewScale = m_SwipeOriginScale;
                NewScale.y *= t;

                m_FireSwipe[i]->Transform()->SetLocalScale(NewScale);
            }
        }
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoAtkA_DoubleSwordAtkLR::End()
{
    // 이펙트 해제
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 15))
    {
        m_bFrmEnter = false;

        for (int i = 0; i < 2; ++i)
        {
            if (m_LightningEffect[i])
            {
                CChangeAlphaScript* Script = m_LightningEffect[i]->GetScript<CChangeAlphaScript>();
                Script->FadeOutDestroy(1.f);
                m_LightningEffect[i] = nullptr;
            }
        }
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::EndWait);
    }
}

void CMorphoAtkA_DoubleSwordAtkLR::EndWait()
{
    float Sim = 0.f;

    // swipe fire
    for (int i = 0; i < 2; ++i)
    {
        if (m_FireSwipe[i])
        {
            m_FireSwipe[i]->Transform()->Slerp(m_SwipeTargetDir, 3.f * DT);
            Sim = m_FireSwipe[0]->Transform()->GetWorldDir(DIR_TYPE::FRONT).Dot(m_SwipeTargetDir);
        }
    }

    // 1도 이하로 줄어들면 Burning
    if (acos(Sim) < (XM_PI / 180.f) && !m_bOverlapBurning)
    {
        m_bOverlapBurning = true;

        for (int i = 0; i < 2; ++i)
        {
            if (m_FireSwipe[i])
            {
                const vector<CGameObject*>& vecChild = m_FireSwipe[i]->GetChildObject();
                for (int j = 0; j < vecChild.size(); ++j)
                {
                    CGameObject* pFireParticle = vecChild[j]->GetChildObject(L"MorphoFireParticle");
                    if (nullptr != pFireParticle)
                    {
                        CParticleSystem* pParticleSystem = pFireParticle->ParticleSystem();

                        if (nullptr != pParticleSystem)
                        {
                            tParticleModule Module = pParticleSystem->GetParticleModule();
                            Module.vSpawnMinScale *= 2.f;
                            Module.vSpawnMaxScale *= 2.f;
                            Module.MinLife = 0.5f;
                            Module.MaxLife = 0.8f;
                            Module.MinSpeed = 180.f;
                            Module.MaxSpeed = 180.f;
                            pParticleSystem->SetParticleModule(Module);
                        }
                    }
                }
            }
        }

        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\DoubleSwordFireBurning.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 1.3f);

        CAMERACTRL->Shake(0.3f, 20.f, 20.f);
    }

    if (Sim >= 1.f - 1e-5f)
    {
        ChangeStep(StateStep::EndEnd);
    }
}

void CMorphoAtkA_DoubleSwordAtkLR::EndEnd()
{
    m_AccTime += DT;

    float DurationTime = 1.f;
    if (m_AccTime > DurationTime)
    {
        MRPFSM->Move();
    }
}