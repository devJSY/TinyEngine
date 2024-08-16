#include "pch.h"
#include "CMorphoShockWave.h"
#include "CPlayerMgr.h"

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

    //@CAMERA 회전 (30s)
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