#include "pch.h"
#include "CElfilisA_StabCombo.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"

CElfilisA_StabCombo::CElfilisA_StabCombo()
{
}

CElfilisA_StabCombo::~CElfilisA_StabCombo()
{
}

void CElfilisA_StabCombo::tick()
{
}

void CElfilisA_StabCombo::Enter()
{
    ELFFSM->SetPattern(ElfilisPatternType::StabCombo);
    ELFFSM->ProcPatternStep();
}

void CElfilisA_StabCombo::Exit()
{
}