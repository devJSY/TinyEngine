#include "pch.h"
#include "CMorphoAtkG_NormalFar_SwordSlash.h"
#include "CMorphoFSM.h"
#include "CMomentaryObjScript.h"
#include "CCameraController.h"

CMorphoAtkG_NormalFar_SwordSlash::CMorphoAtkG_NormalFar_SwordSlash()
    : m_SwordSlash{nullptr,}
    , m_SlashSpeed(750.f)
    , m_bFrmEnter(true)
{
    m_SwordSlashPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\MorphoSwordSlash.pref", L"prefab\\MorphoSwordSlash.pref");
}

CMorphoAtkG_NormalFar_SwordSlash::~CMorphoAtkG_NormalFar_SwordSlash()
{
}

void CMorphoAtkG_NormalFar_SwordSlash::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Combo1: {
        Combo1();
    }
    break;
    case StateStep::Combo2: {
        Combo2();
    }
    break;
    case StateStep::Combo3: {
        Combo3();
    }
    break;
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CMorphoAtkG_NormalFar_SwordSlash::Exit()
{
    Exit_Step();

    MRPFSM->OffWeaponRTrigger();

    for (int i = 0; i < 3; ++i)
    {
        m_SwordSlash[i] = nullptr;
    }
}

void CMorphoAtkG_NormalFar_SwordSlash::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack1Start"), false, false, 1.5f);
    }
    break;
    case StateStep::Combo1: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack1"), false, false, 1.5f);
        MRPFSM->OnWeaponRTrigger();
        m_bFrmEnter = true;

        // spawn sword slash
        m_SwordSlash[0] = CreateSwordSlash(Vec3(-16.78f, 14.8f, 24.08f), 15.f);

        if (m_SwordSlash[0])
        {
            m_SwordSlash[0]->SetName(L"MorphoSwordSlash1");
            GamePlayStatic::SpawnGameObject(m_SwordSlash[0], LAYER_MONSTERATK);
        }
    }
    break;
    case StateStep::Combo2: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack2"), false, false, 1.5f);
        m_bFrmEnter = true;

        // spawn sword slash
        m_SwordSlash[1] = CreateSwordSlash(Vec3(-6.61f, 14.8f, 32.24f), 15.26f);

        if (m_SwordSlash[1])
        {
            m_SwordSlash[1]->SetName(L"MorphoSwordSlash2");
            GamePlayStatic::SpawnGameObject(m_SwordSlash[1], LAYER_MONSTERATK);
        }
    }
    break;
    case StateStep::Combo3: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack3"), false, false, 1.5f);
        m_bFrmEnter = true;

        // spawn sword slash
        m_SwordSlash[2] = CreateSwordSlash(Vec3(0.f, 14.8f, 24.f), 0.f);

        if (m_SwordSlash[2])
        {
            m_SwordSlash[2]->SetName(L"MorphoSwordSlash3");
            GamePlayStatic::SpawnGameObject(m_SwordSlash[2], LAYER_MONSTERATK);
        }
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack3End"), false, false, 1.5f);
    }
    break;
    }
}

void CMorphoAtkG_NormalFar_SwordSlash::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Combo1:
        break;
    case StateStep::Combo2:
        break;
    case StateStep::Combo3:
        break;
    case StateStep::End:
        break;
    }
}

void CMorphoAtkG_NormalFar_SwordSlash::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Combo1);
    }
}

void CMorphoAtkG_NormalFar_SwordSlash::Combo1()
{
    RotateToPlayer();

    // Shoot Slash
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 8))
    {
        m_bFrmEnter = false;

        if (m_SwordSlash[0])
        {
            Vec3 Dir = m_SwordSlash[0]->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            Dir.y = 0.f;
            Dir.Normalize();
            Vec3 Force = Dir * m_SlashSpeed;

            m_SwordSlash[0]->Rigidbody()->AddForce(Force, ForceMode::Impulse);
            m_SwordSlash[0]->Rigidbody()->SetFreezeRotation(AXIS_TYPE::X, true);
            m_SwordSlash[0]->Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
            m_SwordSlash[0]->Rigidbody()->SetFreezeRotation(AXIS_TYPE::Z, true);
        }

        CAMERACTRL->Shake(0.3f, 20.f, 20.f);
    }

    // Change Step
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Combo2);
    }
}

void CMorphoAtkG_NormalFar_SwordSlash::Combo2()
{
    RotateToPlayer();

    // Shoot Slash
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 13))
    {
        m_bFrmEnter = false;

        if (m_SwordSlash[1])
        {
            Vec3 Dir = m_SwordSlash[1]->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            Dir.y = 0.f;
            Dir.Normalize();
            Vec3 Force = Dir * m_SlashSpeed;

            m_SwordSlash[1]->Rigidbody()->AddForce(Force, ForceMode::Impulse);
            m_SwordSlash[1]->Rigidbody()->SetFreezeRotation(AXIS_TYPE::X, true);
            m_SwordSlash[1]->Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
            m_SwordSlash[1]->Rigidbody()->SetFreezeRotation(AXIS_TYPE::Z, true);
        }

        CAMERACTRL->Shake(0.3f, 20.f, 20.f);
    }

    // Change Step
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Combo3);
    }
}

void CMorphoAtkG_NormalFar_SwordSlash::Combo3()
{
    RotateToPlayer();

    // Shoot Slash
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 20))
    {
        m_bFrmEnter = false;

        if (m_SwordSlash[2])
        {
            Vec3 Dir = m_SwordSlash[2]->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            Dir.y = 0.f;
            Dir.Normalize();
            Vec3 Force = Dir * m_SlashSpeed * 1.6f;

            m_SwordSlash[2]->Rigidbody()->AddForce(Force, ForceMode::Impulse);
            m_SwordSlash[2]->Rigidbody()->SetFreezeRotation(AXIS_TYPE::X, true);
            m_SwordSlash[2]->Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
            m_SwordSlash[2]->Rigidbody()->SetFreezeRotation(AXIS_TYPE::Z, true);
        }

        CAMERACTRL->Shake(0.3f, 20.f, 20.f);
    }

    // Change Step
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoAtkG_NormalFar_SwordSlash::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        MRPFSM->Move();
    }
}

CGameObject* CMorphoAtkG_NormalFar_SwordSlash::CreateSwordSlash(Vec3 _PosOffset, float _RotZ)
{
    CGameObject* pSlash = nullptr;

    if (m_SwordSlashPref != nullptr)
    {
        pSlash = m_SwordSlashPref->Instantiate();
        CMomentaryObjScript* pScript = pSlash->GetScript<CMomentaryObjScript>();

        Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
        Pos += GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * _PosOffset.z;
        Pos += GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * _PosOffset.x;
        Pos.y += _PosOffset.y;
        pSlash->Transform()->SetWorldPos(Pos);

        pSlash->Transform()->Slerp(GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT), 1.f);
        Vec3 Rot = pSlash->Transform()->GetWorldRotation();
        Rot.z -= XMConvertToRadians(_RotZ);
        pSlash->Transform()->SetWorldRotation(Rot);

        pScript->SetPlayTime(3.f);
        pScript->SetScaling(Vec3(1.f), 0.2f);
    }

    return pSlash;
}