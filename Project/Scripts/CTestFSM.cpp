#include "pch.h"
#include "CTestFSM.h"

#include "CTestForward.h"
#include "CTestIdle.h"
#include "CTestLeft.h"
#include "CTestRight.h"

CTestFSM::CTestFSM()
	:CFSMScript(TESTFSM)
{
}

CTestFSM::~CTestFSM()
{
}

void CTestFSM::begin()
{
	CState* pState = nullptr;

	pState = new CTestForward;
	AddState(L"FORWARD", pState);


	pState = new CTestIdle;
	AddState(L"IDLE", pState);


	pState = new CTestLeft;
	AddState(L"LEFT", pState);


	pState = new CTestRight;
	AddState(L"RIGHT", pState);

	ChangeState(L"IDLE");
}

