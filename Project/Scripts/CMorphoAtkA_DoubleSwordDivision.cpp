#include "pch.h"
#include "CMorphoAtkA_DoubleSwordDivision.h"
#include "CMorphoFSM.h"

CMorphoAtkA_DoubleSwordDivision::CMorphoAtkA_DoubleSwordDivision()
    : m_AccTime(0.f)
    , m_ChargeTime(1.5f)
    , m_WaitTime(0.5f)
{
}

CMorphoAtkA_DoubleSwordDivision::~CMorphoAtkA_DoubleSwordDivision()
{
}

void CMorphoAtkA_DoubleSwordDivision::tick()
{
    switch (m_Step)
    {
    case StateStep::Charge: {
        Charge();
    }
    break;
    case StateStep::ChargeWait: {
        ChargeWait();
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
    }
}

void CMorphoAtkA_DoubleSwordDivision::Enter()
{
    m_Step = StateStep::Charge;
    Enter_Step();
}

void CMorphoAtkA_DoubleSwordDivision::Exit()
{
    Exit_Step();

    GetOwner()->Rigidbody()->SetUseGravity(true);
    if (m_Step < StateStep::Progress)
    {
        MRPFSM->ResetEmissive();
    }

    // sound
    wstring Sound = L"sound\\wav\\CharaMorphoknight\\DoubleSwordCharging.wav";
    GamePlayStatic::StopSound(Sound);
}

void CMorphoAtkA_DoubleSwordDivision::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Charge: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordDivisionChargeStart"), false, false, 1.5f);
        GetOwner()->Rigidbody()->SetUseGravity(false);

        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\DoubleSwordCharging.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO);
    }
    break;
    case StateStep::ChargeWait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordDivisionCharge"), true, false, 1.5f);
        m_AccTime = 0.f;
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordDivision"), false, false, 1.5f);

        MRPFSM->GetWeaponL()->SetActive(true);

        // sound
        wstring SoundCharging = L"sound\\wav\\CharaMorphoknight\\DoubleSwordCharging.wav";
        GamePlayStatic::StopSound(SoundCharging);

        wstring Sound = L"sound\\wav\\CharaMorphoknight\\Charging1.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 1.2f);

    } break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordWait"), true, false, 1.5f);
        m_AccTime = 0.f;
    }
    break;
    }
    }

    void CMorphoAtkA_DoubleSwordDivision::Exit_Step()
    {
        switch (m_Step)
        {
        case StateStep::Charge:
            break;
        case StateStep::ChargeWait:
            break;
        case StateStep::Progress: {
            MRPFSM->ResetEmissive();
        }
        break;
        case StateStep::End:
            break;
        }
    }

    void CMorphoAtkA_DoubleSwordDivision::Charge()
    {
        if (GetOwner()->Animator()->IsFinish())
        {
            ChangeStep(StateStep::ChargeWait);
        }
    }

    void CMorphoAtkA_DoubleSwordDivision::ChargeWait()
    {
        m_AccTime += DT;

        // emissive
        float t1 = m_AccTime / m_ChargeTime;
        float t2 = sinf(t1 * XM_PI / 2.f);
        Vec3 Color(t2, t1, t1);
        MRPFSM->SetWeaponEmissive(Color);

        if (m_AccTime > m_ChargeTime)
        {
            ChangeStep(StateStep::Progress);
        }
    }

    void CMorphoAtkA_DoubleSwordDivision::Progress()
    {
        // emissive
        float t1 = GetOwner()->Animator()->GetClipPlayRatio();
        float t2 = cosf(t1 * XM_PI / 2.f);
        t1 = 1.f - t1;
        Vec3 Color(t2, t1, t1);
        MRPFSM->SetWeaponEmissive(Color);

        if (GetOwner()->Animator()->IsFinish())
        {
            ChangeStep(StateStep::End);
        }
    }

    void CMorphoAtkA_DoubleSwordDivision::End()
    {
        m_AccTime += DT;

        if (m_AccTime > m_WaitTime)
        {
            MRPFSM->ProcPatternStep();
        }
    }