#include "pch.h"
#include "CMorphoShockWave.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CState.h"
#include "CCameraController.h"

CMorphoShockWave::CMorphoShockWave()
    : CScript(MORPHOSHOCKWAVE)
    , m_AccTime(0.f)
    , m_SpawnDuration(0.2f)
    , m_bSpawnWave(true)
{
    m_WavePref[0] = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_MorphoShockWaveWave_Pink.pref");
    m_WavePref[1] = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_MorphoShockWaveWave_Yellow.pref");
}

CMorphoShockWave::~CMorphoShockWave()
{
}

void CMorphoShockWave::tick()
{
    if (!m_bSpawnWave)
        return;

    m_AccTime += DT;

    if (m_AccTime >= m_SpawnDuration)
    {
        m_AccTime = 0.f;
        SpawnWave();
    }
}

void CMorphoShockWave::Activate()
{
    m_AccTime = m_SpawnDuration;
    m_bSpawnWave = true;
    GetOwner()->SetActive(true);
}

void CMorphoShockWave::Deactivate()
{
    m_bSpawnWave = false;
    GetOwner()->SetActive(false);
}

void CMorphoShockWave::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner() != PLAYER)
        return;

    if (PLAYERFSM->IsInvincible() || PLAYERFSM->GetCurState()->GetName().find(L"DODGE") != wstring::npos)
        return;

    // Camera : 회전
    CAMERACTRL->Shake(0.3f, 30.f, 30.f);
    CAMERACTRL->Tilt(10.f, 1.5f);

    //@EFFECT 커비 디버프 효과
}

void CMorphoShockWave::SpawnWave()
{
    CGameObject* Wave = nullptr;
    int Rand = GetRandomInt(0, 1);
    
    if (Rand == 0)
    {
        Wave = m_WavePref[0]->Instantiate();
    }
    else if (Rand == 1)
    {
        Wave = m_WavePref[1]->Instantiate();
    }

    Wave->Transform()->SetWorldPos(Transform()->GetWorldPos());
    GamePlayStatic::SpawnGameObject(Wave, LAYER_EFFECT);
}

UINT CMorphoShockWave::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CMorphoShockWave::LoadFromLevelFile(FILE* _File)
{
    return 0;
}