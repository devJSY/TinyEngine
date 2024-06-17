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
    CKirbyFSM* KirbyFSM = dynamic_cast<CKirbyFSM*>(GetOwnerFSM());
    
    KirbyFSM->m_CurAbility->Run();
}

void CKirbyRun::Enter()
{
}

void CKirbyRun::Exit()
{
}
