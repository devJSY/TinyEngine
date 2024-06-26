#include "pch.h"
#include "CKirbyRun.h"

#include "CKirbyFSM.h"
#include "CKirbyAbility.h"

CKirbyRun::CKirbyRun()
{
}

CKirbyRun::~CKirbyRun()
{
}

void CKirbyRun::tick()
{
    CKirbyFSM* KirbyFSM = nullptr;
    //CKirbyFSM* KirbyFSM = dynamic_cast<CKirbyFSM*>(GetOwnerFSM());
    //PlayerManager 따위를 둠 -> manager가 player를 들고 있고, 그걸 가져오는게 나을듯 (level 또는 게임 시작시 1회 찾음)

    if (KirbyFSM->GetCurObject())
    {
        // object가 있다면 이거 우선
    }
    else
    {
        // 기본적으로 수행해야 하는 동작

        // 현재 state에 맞는 추가동작
        KirbyFSM->GetCurAbility()->Run();
    }
}

void CKirbyRun::Enter()
{
}

void CKirbyRun::Exit()
{
}
