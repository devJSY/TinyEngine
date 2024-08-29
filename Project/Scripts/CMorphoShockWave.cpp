#include "pch.h"
#include "CMorphoShockWave.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CState.h"
#include "CCameraController.h"

CMorphoShockWave::CMorphoShockWave()
    : CScript(MORPHOSHOCKWAVE)
{
}

CMorphoShockWave::~CMorphoShockWave()
{
}

void CMorphoShockWave::tick()
{
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

UINT CMorphoShockWave::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CMorphoShockWave::LoadFromLevelFile(FILE* _File)
{
    return 0;
}