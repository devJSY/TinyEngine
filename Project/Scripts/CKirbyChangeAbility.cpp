#include "pch.h"
#include "CKirbyChangeAbility.h"

CKirbyChangeAbility::CKirbyChangeAbility()
{
}

CKirbyChangeAbility::~CKirbyChangeAbility()
{
}

void CKirbyChangeAbility::tick()
{
    // 애니메이션 재생중 필요한 작업 
    PLAYERFSM->GetNextAbility()->ChangeAbility();

    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"CHANGE_ABILITY_WAIT");
    }

}

void CKirbyChangeAbility::Enter()
{
    // 소켓 꽂아주기
    PLAYERFSM->GetNextAbility()->ChangeAbilityEnter();
    
    // 커비 변신 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"EvolutionCopy"), false, false, 1.5f);

    // 커버 머터리얼 다시 설정
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthSmileOpen));
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    // @TODO 
    // 카메라 조절
    // 커비가 카메라쪽을 바라보게 회전
    // 배경 블러 효과
    // 커비 움직임 제한
    // 커비를 제외한 모든 오브젝트가 멈추어야 함
}

void CKirbyChangeAbility::Exit()
{
    PLAYERFSM->GetNextAbility()->ChangeAbilityExit();
}
