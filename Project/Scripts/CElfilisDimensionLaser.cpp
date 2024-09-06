#include "pch.h"
#include "CElfilisDimensionLaser.h"
#include "CElfilisLaser.h"

CElfilisDimensionLaser::CElfilisDimensionLaser()
    : CScript(ELFILISDIMENSIONLASER)
    , m_Laser(nullptr)
    , m_Step(0)
    , m_AccTime(0.f)
    , m_PlayTime(3.f)
    , m_bLaserStartLock(false)
    , m_bLaserRepeat(false)
{
}

CElfilisDimensionLaser::~CElfilisDimensionLaser()
{
}

void CElfilisDimensionLaser::begin()
{
    if (!m_Laser)
    {
        ParseChild();
    }
}

void CElfilisDimensionLaser::tick()
{
    switch (m_Step)
    {
    case 0:
        break;
    case 1:
        Spawn();
        break;
    case 2:
        Wait();
        break;
    case 3:
        StartAttack();
        break;
    case 4:
        Progress();
        break;
    case 5:
        EndAttack();
        break;
    case 6:
        Disappear();
        break;
    case 7:
        break;
    }
}

void CElfilisDimensionLaser::PlaySpawn()
{
    if (!m_Laser)
    {
        ParseChild();
    }
    m_Step = 1;
    m_AccTime = 0.f;

    m_Laser->GetOwner()->SetActive(false);
    for (CGameObject* iter : m_vecDimension)
    {
        iter->SetActive(true);
    }
}

void CElfilisDimensionLaser::PlayStartLaser(float _PlayTime)
{
    m_Step = 3;
    m_PlayTime = _PlayTime;

    m_Laser->SetStart();
    m_Laser->GetOwner()->SetActive(true);
}

void CElfilisDimensionLaser::PlayEndLaser()
{
    m_Step = 5;
    m_Laser->SetEnd();
}

void CElfilisDimensionLaser::Reset()
{
    m_Step = 0;
    m_AccTime = 0.f;
    m_PlayTime = 0.f;
    m_bLaserStartLock = false;
    m_bLaserRepeat = false;
}

void CElfilisDimensionLaser::Spawn()
{
    float SpawnTime = 0.3f;
    m_AccTime += DT;

    if (m_AccTime <= SpawnTime)
    {
        float t = m_AccTime / SpawnTime;
        for (CGameObject* iter : m_vecDimension)
        {
            iter->Transform()->SetLocalScale(Vec3(t));
        }
    }
    else
    {
        for (CGameObject* iter : m_vecDimension)
        {
            iter->Transform()->SetLocalScale(Vec3(1.f));
        }

        m_Step = 2;
        m_AccTime = 0.f;
    }
}

void CElfilisDimensionLaser::Wait()
{
    if (m_bLaserStartLock)
        return;

    m_AccTime += DT;

    if (m_AccTime > 1.f)
    {
        m_Step = 3;
        m_Laser->SetStart();
        m_Laser->GetOwner()->SetActive(true);

        // sound
        wstring SoundKeyLaserStart = L"sound\\wav\\CharaBossChimeraSoul\\0014_DimensionLaser_LaserReady.wav";
        GamePlayStatic::Play2DSound(SoundKeyLaserStart, 1, SOUND_ELFILIS * 0.6f, true, false);
    }
}

void CElfilisDimensionLaser::StartAttack()
{
    if (m_Laser->Animator()->IsFinish())
    {
        m_Step = 4;
        m_AccTime = 0.f;
        m_Laser->SetWait();
    }
}

void CElfilisDimensionLaser::Progress()
{
    if (m_bLaserRepeat)
        return;

    m_AccTime += DT;

    if (m_AccTime > m_PlayTime)
    {
        m_Step = 5;
        m_Laser->SetEnd();
    }
}

void CElfilisDimensionLaser::EndAttack()
{
    if (m_Laser->Animator()->IsFinish())
    {
        m_Step = 6;
        m_AccTime = 0.f;
        m_Laser->GetOwner()->SetActive(false);
    }
}

void CElfilisDimensionLaser::Disappear()
{
    float DisappearTime = 0.3f;
    m_AccTime += DT;

    if (m_AccTime <= DisappearTime)
    {
        for (CGameObject* iter : m_vecDimension)
        {
            float t = 1.f - m_AccTime / DisappearTime;
            iter->Transform()->SetLocalScale(Vec3(t));
        }
    }
    else
    {
        for (CGameObject* iter : m_vecDimension)
        {
            iter->SetActive(false);
        }

        m_Step = 7;
        m_AccTime = 0.f;
    }
}

void CElfilisDimensionLaser::ParseChild()
{
    vector<CGameObject*> Childs = GetOwner()->GetChildObject();

    for (CGameObject* iter : Childs)
    {
        if (iter->GetName() == L"DimensionGate")
        {
            m_vecDimension.push_back(iter);
        }
        else if (iter->GetName() == L"DimensionLaser")
        {
            CElfilisLaser* Laser = iter->GetScript<CElfilisLaser>();
            m_Laser = Laser;
        }
    }
}

UINT CElfilisDimensionLaser::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CElfilisDimensionLaser::LoadFromLevelFile(FILE* _File)
{
    return 0;
}