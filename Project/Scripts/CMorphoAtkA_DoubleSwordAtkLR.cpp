#include "pch.h"
#include "CMorphoAtkA_DoubleSwordAtkLR.h"
#include "CMorphoFSM.h"
#include "CPlayerMgr.h"
#include "CCameraController.h"
#include "CChangeAlphaScript.h"

CMorphoAtkA_DoubleSwordAtkLR::CMorphoAtkA_DoubleSwordAtkLR()
    : m_LightningEffect{nullptr,}
    , m_FireSwipe{nullptr,}
    , m_bFrmEnter(true)
{
    m_LightningEffectPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_MorphoLightningSet.pref", L"prefab\\Effect_MorphoLightningSet.pref");
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
    }

    if (m_Step < StateStep::EndEnd)
    {
        m_Step = StateStep::EndEnd;
        Exit_Step();
    }
}

void CMorphoAtkA_DoubleSwordAtkLR::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack3Start"), false, false, 1.5f);
        //@EFFECT 차징이펙트
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack3"), false, false, 1.5f);
        m_bFrmEnter = true;
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack3End"), false, false, 1.5f);
        m_bFrmEnter = true;
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
    case StateStep::Start:
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
    case StateStep::EndWait:
        break;
    case StateStep::EndEnd: {
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
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoAtkA_DoubleSwordAtkLR::Progress()
{
    int SpawnFrm = 20;

    //@EFFECT 번개, 나비, 충격파
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), SpawnFrm))
    {
        m_bFrmEnter = false;

        // spawn lightning
        if (m_LightningEffectPref != nullptr)
        {
            Vec3 Dir = CPlayerMgr::GetCameraController()->GetLookDir() * -1.f;
            Dir.y = 0.f;

            // Left
            m_LightningEffect[0] = m_LightningEffectPref->Instantiate();
            Vec3 Pos = MRPFSM->GetWeaponL()->GetChildObject()[0]->Transform()->GetWorldPos();
            Pos.y = 0.f;

            m_LightningEffect[0]->Transform()->SetWorldPos(Pos);
            m_LightningEffect[0]->Transform()->Slerp(Dir.Normalize(), 1.f);

            GamePlayStatic::SpawnGameObject(m_LightningEffect[0], LAYER_EFFECT);

            // Right
            m_LightningEffect[1] = m_LightningEffectPref->Instantiate();
            Pos = MRPFSM->GetWeaponR()->GetChildObject()[0]->Transform()->GetWorldPos();
            Pos.y = 0.f;

            m_LightningEffect[1]->Transform()->SetWorldPos(Pos);
            m_LightningEffect[1]->Transform()->Slerp(Dir.Normalize(), 1.f);

            GamePlayStatic::SpawnGameObject(m_LightningEffect[1], LAYER_EFFECT);
        }

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
        }
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

    if (Sim >= 1.f - 1e-5f)
    {
        ChangeStep(StateStep::EndEnd);
    }
}

void CMorphoAtkA_DoubleSwordAtkLR::EndEnd()
{
    m_AccTime += DT;

    float DurationTime = 0.3f;
    float BurningTime = 1.f;

    // start big
    if (m_AccTime <= DurationTime)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (m_FireSwipe[i])
            {
                float t = 1.f + (m_AccTime / DurationTime) * 1.5f;
                Vec3 NewScale = m_SwipeOriginScale;
                NewScale.y *= t;
                m_FireSwipe[i]->Transform()->SetLocalScale(NewScale);
            }
        }
    }

    // burning
    else if (m_AccTime < DurationTime + BurningTime)
    {
        if (m_FireSwipe[0])
        {
            m_SwipeOriginScale = m_FireSwipe[0]->Transform()->GetLocalScale();
        }
    }

    // start end
    else if (m_AccTime < 2.f * DurationTime + BurningTime)
    {
        for (int i = 0; i < 2; ++i)
        {
            float t = 1.f - ((m_AccTime - DurationTime - BurningTime) / DurationTime);
            Vec3 NewScale = m_SwipeOriginScale;
            NewScale.y *= t;
            m_FireSwipe[i]->Transform()->SetLocalScale(NewScale);
        }
    }

    // end
    else
    {
        MRPFSM->Move();
    }
}
