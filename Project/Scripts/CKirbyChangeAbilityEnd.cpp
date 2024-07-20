#include "pch.h"
#include "CKirbyChangeAbilityEnd.h"

CKirbyChangeAbilityEnd::CKirbyChangeAbilityEnd()
{
}

CKirbyChangeAbilityEnd::~CKirbyChangeAbilityEnd()
{
}

void CKirbyChangeAbilityEnd::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }

}

void CKirbyChangeAbilityEnd::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("EvolutionCopyEnd"), false, false, 1.5f);
}

void CKirbyChangeAbilityEnd::Exit()
{
    // 커비 표정 복구
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    //@TODO
    //카메라 원상 복구
    //배경 블러 효과 복구
    //커비 움직임 제한 복구
    //커비를 의외의 오브젝트 다시 DT받도록 수정

}
